#include "thread.hpp"
#include "array_algos.hpp"

namespace doot{

extern void _thread(char const*, FPTR(_,void*,void),void*);
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
vector<thread_loop> warp_threads;
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
	arr<void*> in;
	vector<void*>& out;
};

void _invoke(
			lambda_opaq f,
			siz stride_a,
			arr<void*> in,
			vector<vector<void*>>& out ){
	if(active){
		warn("warp double dispatch, check reentrancy");
		re;
	}
	active= true;

	u8* base= (u8*)in.base;
	u8* stop= (u8*)in.stop;
	siz stride= stride_a;

	sizt total= ((siz)(stop-base))/stride;//number of T
	sizt denom= poolsize;//denominator, number of threads
	vector<warp_task_io_t> segs(denom);//segments

	//vec<vec<void> -> vec<vec<u8>>
	//inner vector doesnt need type for outer vector to have correct stride
	// as vector members are only pointers, which must be samely sized
	vector<vector<void*>>& segs_out= *out;

	if(denom<=0){
		warn("thread::warp::dispatch:: 0 jobs provided");
		return;
	}
	if(
		WARP_NO_MULTITHREAD
		|| poolsize==0 || poolsize==1 || total==1 ){
			vector<void*>& out_= out->add(void);
			call_opaq_t call{f,warp_task_io_t{in,out_}};
			re call.invoke();
	}

	ass(denom==poolsize & denom==warp_threads.size());

	vector<pair<siz>> indxss;//untyped offsets

	//distribute
	if(denom>total){//more jobs than threads
		siz span= total/denom;//per thread
		siz rem= total - span*denom;//remainder
		ass(rem>=0);
	
		RA(i,denom){
			indxss.add({
				i*span,
				b+span
			});
		}
		//give remainder to last thread
		//worstcase: total= denom*2-1
		//	todo the remainder should be dsitributed evenly
		indxs[denom-1].b+= rem;//b:=stop
	
	}else{//less jobs than threads
		//remainder of threads will not dispatch
		RA(i,total){
			indxss.add({
				i  ,
				i+1
			});
		}
	}

	EACH(idxs,idxss){
		segs.add({
			arr<void>{
				(void*)(((u8*)idxs.a)*stride + base) ,
				(void*)(((u8*)idxs.b)*stride + base) },
			segs_out.add(void)
		});
	}

	//woken threads
	ltch.set(segs.size());
	EN(i,seg,segs){ //iterate segs not threads since there may be less 
		bool busy=
			warp_threads[i].dispatch({ f, &seg });
		if(busy)
			warn("thread::warp::dispatch thread not free");
	}
	ltch.wait();
	active= false;
}



}

}
