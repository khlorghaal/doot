#include "thread.hpp"

namespace doot{

extern void _thread(char const*, FPTR(_,void*,void),void*);
void thread(str name, thread_arg tharg){
	_thread(name, tharg.f, tharg.x);
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



struct thread_loop{
	lock lck;
	mutex mut;
	volatile bool aktiv= true;
	thread_arg order;


	static void loop_static(void* that){
		((thread_loop*)that)->loop();
	}
	thread_loop(str name){
		thread(name,thread_arg{loop_static,this});
	}

	bool dispatch(thread_arg tharg){
		mut.lock();
			if(aktiv){
				mut.locknt();
				return true;
			}
			order= tharg;
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
				thread_arg ord= order;
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


//fucking unsafe holy shit
//this is most unsafe code ive ever written
//12 year old me wrote safer code
void _dispatch(i8* base, i8* stop, sizt stride, FPTR(task,void*,void)){
	ass(!active);
	active= true;

	sizt total= (sizt)(stop-base)/stride;//number of T
	sizt denom= poolsize;//denominator, number of threads
	vector<arr<i8>> segs(denom);//task casts back to arr<T>

	if(denom<=0){
		warn("thread::warp::dispatch:: 0 jobs provided");
		return;
	}
	if(
		WARP_NO_MULTITHREAD
		|| poolsize==0
		|| total==1 ){
			auto jobs= pair<void*>{base,stop};
			task(&jobs);
			return;
	}

	ass(denom==poolsize & denom==warp_threads.size());


	//distribute
	if(denom>total){	
		sizt span= total/denom;//per thread
		sizt rem= total - span*denom;//remainder
		ass(rem>=0);
	
		RA(i,denom){
			i8* b= (i8*)(i*span);
			i8* s= (i8*)(b+span);
			segs.add(arr<i8>{b,s});
		}
		//give remainder to last thread
		//worstcase: total= denom*2-1
		//	todo the remainder should be dsitributed evenly
		segs[denom-1].stop+= rem;
	
	}else{//less jobs than threads
		RA(i,total){
			segs.add(arr<i8>{
				(i8*)i  ,
				(i8*)i+1});
		}
	}
	EACH(s,segs){
		s.base= ((sizt)s.base)*stride + base;
		s.stop= ((sizt)s.stop)*stride + base;
	}


	//woken threads
	ltch.set(denom);
	EN(i,thr,warp_threads){
		bool busy=
			thr.dispatch(
				thread_arg{
					(void(*)(void*))((void*)&task),
					&segs[i]
				});//this nearly made me cry
		if(busy)
			bad("thread::warp::dispatch fucksy wucksy");
	}

	ltch.wait();
	active= false;
}



}

}


/* ass pattern
class executor{
	lock lock;
	mutex mut;
	volatile bool alive= true;

	struct order_t{
		fpvvp fun;
		void* arg;
		funp<void*> arg_dtor;;//nullable
		latch* latch;//nullable

		void invoke(){
			fun(arg);
			if(!!latch){
				latch->tick();
				latch->await();
			}
		};
		void destruct(){
			if(!!arg_dtor)
				arg_dtor(arg);
		}
	};
	vector<order_t> orders;

	static void loop_static(void* that){
		((executor*)that)->loop();
	}
	void loop(){
		lock.await();

		while(true){
			mut.lock();
			bool alive_= alive;
			mut.locknt();
			if(!alive_)
				return;

			mut.lock();
			order_t ord= orders.pop();
			mut.locknt();
			ord.invoke();
			ord.destruct();

			lock.await();
		}
	}

public:
	executor(str& name){
		thread(name,loop_static,this);
	}
	~executor(){
		mut.lock();
		alive= false;
		ass(orders.empty());
		mut.locknt();
		lock.wake();
	}

	void order(fpvvp fun, void* arg, latch* latch){
		mut.lock();
		orders.push_front({fun, arg, latch});
		mut.locknt();
		lock.wake();
	}
	void order(fpvvp fun_,void* arg){ order(fun_,arg, NULL); }
	void order(fpvvp fun_)          { order(fun_,NULL,NULL); }
};
*/