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
#include "vector.hpp"

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


struct _invocation_voidp{
	virtual void op()(void*)=0;
};
tpl<typn A, typn B>
struct _invocation: _invocation_voidp{
	struct pack{
		   arr<A>* a;
		vector<B>* b;
	};
	fptr<arr<A>&,vector<B>&,void> f;
	
	virtual void op()(void* p){
		pack* ab= (pack*)p;
		f(ab->a,ab->b);
	}
};
//{arr<T>,vec<T>} -> void* -> {arr<void>,vec<void>} -> {arr<T>,vec<T>}
//caller          -> impl  -> dispatch              -> lambda
extern void _invoke(
	_invocation_void* f,
	siz stride_a,//stride_b not necessary, the lambda is responsible
	          arr<void*> ** in,
	vector<vector<void*>>** out
	//truly not how the language should be used
	//"youre fuckin with god"-shika
	//void* should be considered void, making it a pointer is a hack
	//it could be any type, since arr and varr members are pointers, all pointers are u64
);


/*
In Place Transform
uses the calling thread + pool
blocking, no external synchro needed

each thread is given an arr<T> and vector<T>
	handled by the system
warp itself does not handle segment iteration
	allows optimizer unrolling
the output vector is variable sized
	the kernel may output accordingly

void task(arr<IN>,vec<OUT>)
	for(mut a in arr)
		stuff(mut t)
		stuff(t,vec)//ie elements to be removed or added after
	ret R
use
	arr_raii<IN> i
	vector<OUT> o
	dipatch<IN,OUT,&task>(i,o)

arr<arr<>> is not used
dispatcher passing E implies arr<E>
the parameter is not arr<E> unless a matrix
 the job division requires an array
 each threads array is a segment of the array, not a scalar object
 the task is aware of segments, it is not a scalar kernel

gathering must be done by the invoker using sum

jobs must not be reallocated in any way until complete
	if scoping properly is nonconcern

*/
#define _WARPAR arr<E>&,vector<R>&
tpl<typn E, typn R, FPTR(task,_WARPAR,void)>
void dispatch(arr<E>& in, vector<R>& out){
	//amount of swearing when writing was medium

	//per-thread vector alloced and waived by each thread invocation
	//this is a good use of waiving
	//	it is internal
	//	the resulting lifetimes are ultimately passed to the caller
	
	//basically all semantic complexity due to opaquifying the containers
	//nested vector due from comptime-unknown thread number
	vector<vector<    R>>  vv;
	vector<vector<void*>>& vvv= rcas<vector<vector<void*>>>(&vv);//god is dead i have killed him
	_invocation<E,R> inv{in,out,task};

	//(io -> void) -> (void* -> void)
	_invoke(
		&inv,
		sizeof(E),
		{(void*)in.base,
		 (void*)in.stop},
		vvv);

	//flatten vector of vectors
	EACH(v,vv)
		out+= v;
}

}
}
