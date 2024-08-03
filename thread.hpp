/*
synchro primitives are not exposed
all interthread via
	warp dispatch/gather
	message queues

message queues explicitly consumed within thread loop
	allows easier strong type
*/

#pragma once
#include "doot.hpp"
#include "string.hpp"
#include "list.hpp"
#include "array_algos.hpp"

namespace doot{

/*
doot threading notions constancy
	threads may not terminate
	termination happens with process termination

an application's thread	topology should be
explicitly specified at comptime
therefore all thread objects allocated as globals
	warps do this internally
*/
void thread(str name, call_opaq_t);// mut void*->void

struct mutex{
	OPAQ_M_CDTOR_DECL(mutex);
	void lock();
	void locknt();
};

//includes own mutex and predicate. does not wake spuriously
struct lock{
	OPAQ_M_CDTOR_DECL(lock);
	void wait();//multiple threads may await
	void wake();//all threads waiting
};

//countdown semaphore
struct latch{
	OPAQ_M_CDTOR_DECL(latch);
	void  set(int count);
	void tick();
	void wait();
};

/*one warp per process, unlike gpu warps
warps are invoked sequentially from control thread
threads - init; serial; dispatch; task; gather; task; ...
        - idle;                   task; idle;   task; ...
        - idle;                   task; idle;   task; ...
        - idle;                   task; idle;   task; ...
*/
namespace warp{

void init();

bool cex WARP_NO_MULTITHREAD= true;

/*opaque lambda and its functors, purely semantic complexity
{arr<T>,lis<T>} -> void* -> {arr<void>,lis<void>} -> {arr<T>,lis<T>}
caller          -> impl  -> dispatch              -> lambda  */
struct _invocation_void{
	struct K{
	    arr<void>& a;
		lis<void>& b;
	};
	using KF= FPTR(K,void);
	KF kf;
	using VF= FPTR(void*,void);
	VF vf;
};
tpl<typn A, typn B, FPTR_VAR(f, L2C(arr<A>&,lis<B>&),void)>
struct _invocation : _invocation_void{
	//typed user implementation
	struct AB{
		arr<A>& a;
		lis<B>& b;
	};
	
	static void ftor_AB(AB& ab){
		f(ab.a,ab.b);
	}
	// F->KF
	static void ftor_k(K k){//K<void>->T
		ftor_AB( rcas<AB>(k) );}
	static void ftor_v(void* v){//void*->T
		ftor_AB( *(AB*)v );}

	_invocation(){
		_invocation_void::kf= &ftor_k;
		_invocation_void::vf= &ftor_v;
	}
};
extern void _invoke(
	_invocation_void& f,
       	arr<void> & a,
	lis<lis<void>>& b,//dispatcher emits sublists
	siz stride_a); //stride_b omitted as lambda is responsible
	


/*
In Place Transform
uses the calling thread + pool
blocking, no external synchro needed

each thread is given an arr<T> and list<T>
	handled by the system
warp itself does not handle segment iteration
	allows optimizer unrolling
the output list is variable sized
	the kernel may output accordingly

void task(arr<IN>,vec<OUT>)
	for(mut a in arr)
		stuff(mut t)
		stuff(t,vec)//ie elements to be removed or added after
	ret R
use
	arr_raii<IN> i
	list<OUT> o
	dipatch<IN,OUT,&task>(i,o)

arr<arr<>> is not used
dispatcher passing E implies arr<A>
the parameter is not arr<A> unless a matrix
 the job division requires an array
 each threads array is a segment of the array, not a scalar object
 the task is aware of segments, it is not a scalar kernel

gathering must be done by the invoker using sum

jobs must not be reallocated in any way until complete
	if scoping properly is nonconcern

*/
tpl<typn A, typn B, FPTR_VAR(f, L2C(arr<A>&,lis<B>&), void) >
void dispatch(arr<A>& in, lis<B>& out){

	//amount of swearing when writing was medium

	//per-thread list alloced and waived by each thread invocation
	//this is a good use of waiving
	//	it is internal
	//	the resulting lifetimes are ultimately passed to the caller
	
	//basically all semantic complexity due to opaqueness requirement
	//outer list as thread count comptime-unknown
	lis<lis<B>> llb;

	//(io -> void) -> (void* -> void)
	_invocation<A,B,f> inv;
	auto a= vcas( in);
	auto b=	vcas(llb);
	_invoke(inv,a,b,sizeof(A));

	EACH2(b,llb)
		out+=b;
}

}
}
