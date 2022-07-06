#pragma once
#include "string.hpp"
#include "vector.hpp"

namespace doot{

typedef void (*fpvvp)(void*);
void thread(str& name, fpvvp, void* param);

//this is an uncomfy pattern, but it meets my goal
struct _mutex;
void _mutlock(_mutex*);
void _mutlocknt(_mutex*);
struct mutex{
	_mutex* _;
	mutex();
	~mutex();
	void lock();
	void locknt();
};

//includes own mutex and predicate. does not wake spuriously
struct _lock;
struct lock{
	_lock* _;
	lock();
	~lock();
	void await();//multiple threads may await
	void wake();//all threads waiting
};

struct _latch;
struct latch{
	_latch* _;
	latch();
	~latch();
	void set(int count);
	void tick();
	void await();
};


class executor{
	lock lock;
	mutex mut;
	volatile bool alive= true;

	struct order_t{
		fpvvp fun;
		void* arg;
		latch* latch;//may null

		void invoke(){
			fun(arg);
			if(!!latch){
				latch->tick();
				latch->await();
			}
		};
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
			delete ord.arg;

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



namespace tasker{
#define TASKER_NO_MULTITHREAD true

void init();

extern int poolsize;//excludes invoking working thread
extern volatile bool active;


//number of segments given from size() of ret
//as with all things doot, parameter deletion must be done by outer scope
//result pointers are within the range of jobs
//segments returned are invalid if jobs is reallocated (ie vector modification)
template<typename T>
void segment(arr<T> jobs, arr<arr<T>>& res){
	sizt total= jobs.size();
	sizt denom= res.size();
	ass(denom>0);
	sizt span= total/denom;
	sizt rem= total - span*denom;
	ass(rem>=0);

	for(int i=0; i!=denom; i++){
		arr<T>& s= ret[i];
		s.base= jobs.base + i*span;
		s.stop= s.base + span;
	}
	//give remainder to last thread
	res[denom-1].stop+= rem;
}

void _invoke(fpvvp, arr<void*>);

//i can only concieve this being useful for threading
template<typename T, void(*F)(T)>
void fpvvpwrap(void* p){
	F(*(T*)p);
}

//uses the calling thread + pool
//blocks until completion
//jobs must not be reallocated in any way until complete
template<typename T, void(*task)(arr<T>)>
inline void invoke(arr<T> jobs){
	if(TASKER_NO_MULTITHREAD || poolsize==0){//potato case
		task(jobs);
		return;
	}
	int workers= poolsize+1;
	vector<arr<T>> segs(workers);
	count(i,workers)
		segs.make({0,0});
	segment(jobs,segs);

	_invoke(&fpvvpwrap<arr<T>,task>, &segs);
}

}


}