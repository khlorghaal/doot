#include "thread.hpp"
#include "array_algos.hpp"

namespace doot{

extern void _thread(char const*, FPTR(void*,void),void*);
void thread(str name, call_opaq_t arg){
	_thread(name, arg.f, arg.x);
}



OPAQ_CDTOR_DEFR(mutex);
OPAQEXTRN_M(mutex,lock);
OPAQEXTRN_M(mutex,locknt);


OPAQ_CDTOR_DEFR(lock);
OPAQEXTRN_M(lock,wait);
OPAQEXTRN_M(lock,wake);


OPAQ_CDTOR_DEFR(latch);
extern void latch_set(void*,int);
void latch::set(int i){ latch_set(this,i); };
OPAQEXTRN_M(latch,tick);
OPAQEXTRN_M(latch,wait);

//used to make threads standby after operating
//either for background threads like net/audio
//or to avoid reallocating a thread
struct thread_loop{
	lock lck;
	mutex mut;
	volatile bool aktiv= true;
	call_opaq_t order;

	static void loop_static(void* that){
		((thread_loop*)that)->loop();
	}
	thread_loop(str name){
		thread(name,call_opaq_t{loop_static,this});
	}

	bool dispatch(call_opaq_t arg){
		mut.lock();
			if(aktiv){
				mut.locknt();
				return true;
			}
			order= arg;
		mut.locknt();
		lck.wake();
		return false;
	}

	//given to thread upon construction
	void loop(){
		lck.wait();

		while(true){
			mut.lock();
				aktiv= true;
				call_opaq_t ord= order;
			mut.locknt();

			ord.invoke();

			mut.lock();
			aktiv= false;
			mut.locknt();
			lck.wait();
		}
	}
};

extern int vcore_count(void);

namespace warp{

int poolsize;//excludes invoking worker
volatile bool active= false;

latch ltch;
list<thread_loop> warp_threads;
//the warp loops as a standby after dispatch
//as to not reallocate thread

void init(){
	poolsize= vcore_count();
	
	if(poolsize>2)//more than 2 vcores
		poolsize--;//leave one free
	if(poolsize>0)
		poolsize--;//invoker runs a task on itself
	ass(poolsize>=0);

	RA(i,poolsize)
		warp_threads.add(strfmt("tasker %i",i));
}

struct warp_task_io_t{
	arr<void> in;
	list<void>& out;
};

void _invoke(
	_invocation_void& f, 
       	arr<void> &  in_void,
	lis<lis<void>>& out_void,//dispatcher emits sublists
	siz stride_a){
	if(active){
		warn("warp double dispatch, check reentrancy");
		re;
	}
	active= true;

	arr<u8> in_u8= in_void;
	//can do pointer arithmetic
	//horribly unsafe, remember stride
	//this is not fine, but it will work.

	siz stride= stride_a;
	siz total= in_u8.size()/stride;//number of T
	siz denom= poolsize;//denominator, number of threads
	
	//vec<vec<void> -> vec<vec<u8>> -> vec<vec<void>> -> vec<void> -> vec<T>
	//inner list doesnt need type for outer list to have correct stride
	// as list members are only pointers, which must be samely sized

	if(denom<=0){
		warn("thread::warp::dispatch:: 0 jobs provided");
		re;
	}
	
	if(
		WARP_NO_MULTITHREAD
		|| poolsize==0 || poolsize==1 || total==1 ){
			//todo OPAQ_ call{f,warp_task_io_t{in,out_}};
			list<void>& o= out_void.add();
			f.kf({in_void,o});
			re;
	}

	ass(poolsize==warp_threads.size());
	ass(in_u8.size()%stride==0);//if denom isnt divisible by stride that would result in fragmenting

	list<arr<u8>> segs_indiv;
	div<u8>(in_u8,denom*stride,segs_indiv);

	//multiplying denominator by stride
	//evits need for post-multiplying offsets
	//div function does not need to be aware of stride

	list<warp_task_io_t> segs;
	EACH(sii, segs_indiv){
		arr<void>& siv= rcas<arr<void>>(sii);
		lis<void>& so= out_void.add();
		segs.add(siv,so);
	}

	//woken threads
	ltch.set(segs.size());
	EN(i,seg,segs){ //iterate segs not threads since there may be less 
		bool busy=
			warp_threads[i].dispatch({ f.vf, &seg });
		if(busy)
			warn("thread::warp::dispatch thread not free");
	}
	ltch.wait();
	active= false;
}



}

}
