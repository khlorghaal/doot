#include "tasker.hpp"
#include <cstring>
#include "thread.hpp"
#include <thread>
#include <mutex>
using std::mutex;
#include <condition_variable>
using std::condition_variable;
#include <atomic>
using std::atomic;

namespace doot{

thread_local string thread::name;
thread::thread(string name_, void(*fun)(void*), void* arg){
	name= name_;
	new std::thread(fun, arg);
}


int vcore_count();
#ifdef _WIN32
int vcore_count(){
	return 8;//TODO HACK LOL
};
#elif LINUX

#endif



//includes own mutex and predicate. does not wake spuriously
class lock{
	mutex mut;
	condition_variable cv;
	volatile bool pred= false;
public:
	void await(){
		std::unique_lock<mutex> lck(mut);
		pred= false;
		cv.wait(lck, [this]{return pred;});
	};
	void wake(){
		mut.lock();
		pred= true;
		cv.notify_all();
		mut.unlock();
	}
};

class latch{
	mutex mut;
	condition_variable cv;
	int count= 0;
public:
	void set(int count_){
		mut.lock();
		assert(count==0);
		count= count_;
		mut.unlock();
	}
	void tick(){
		mut.lock();
		if(count>0)
			count--;
		cv.notify_all();
		mut.unlock();
	}
	void await(){
		std::unique_lock<mutex> lck(mut);
		if(count==0)
			return;
		cv.wait(lck, [this]{return count==0;});
	}
};

typedef void (*void_pfn_voidp)(void*);

class thread_persistent: public thread{
	lock lock;
	mutex mut;
	volatile bool alive= true;

	struct order_t{
		void_pfn_voidp fun;
		void* arg;
		latch* latch;//may be null

		void invoke(){
			fun(arg);
			if(!!latch){
				latch->tick();
				latch->await();
			}
		};
	};
	vector<order_t> orders;

	static void loop_static(void* arg){
		((thread_persistent*)arg)->loop();
	}
	void loop(){
		lock.await();

		while(true){
			mut.lock();
			bool alive_= alive;
			mut.unlock();
			if(!alive)
				return;

			mut.lock();
			order_t ord= orders.pop();
			mut.unlock();
			ord.invoke();

			lock.await();
		}
	}

public:
	thread_persistent(string name_)
		: thread(name_, loop_static, this){};
	
	void order(void_pfn_voidp fun, void* arg, latch* latch){
		mut.lock();
		orders.push_front({fun, arg, latch});
		mut.unlock();
		lock.wake();
	}
	void order(void_pfn_voidp fun_, void* arg){ order(fun_, arg,  NULL); }
	void order(void_pfn_voidp fun_)           { order(fun_, NULL, NULL); }

	void kill(){
		mut.lock();
		alive= false;
		assert(orders.empty());
		mut.unlock();
		lock.wake();
	}
};



namespace tasker{

int poolsize;//excludes invoking worker
volatile bool active= false;


//conversion of void fun(arr<T>) -> void fun(void*) for passing to impl
//interface is type-safe
//this is pretty fucking weird
//signatures aren't something that can be discarded, as they are part of assembly
template<typename T, void(*task)(arr<T>)>
void task_voidcast(void* jobs){
	task(*(arr<T>*)jobs);
}

void invoke_impl(void_pfn_voidp, vector<arr_void> const& segs);

template<typename T, void(*task)(arr<T>)>
inline void invoke(arr<T> jobs){
	if(NO_MULTITHREAD || poolsize==0){//potato case
		task(jobs);
		return;
	}
	int workers= poolsize+1;
	arr<arr<T>> segs(workers);
	segment(jobs, segs);

	invoke_impl(&task_voidcast<T, task>, &segs);
}
//</unholy>

::latch latch;
vector<thread_persistent> threads;

void init(){
	poolsize= vcore_count();
	
	if(poolsize>2)//more than 2 vcores
		poolsize--;//leave one free
	if(poolsize>0)
		poolsize--;//invoker runs a task on itself
	assert(poolsize>-1);

	threads.realloc(poolsize);

	int i=0;
	for(auto& thread : threads)
		new (&thread) thread_persistent(string("tasker")<<string(i++));
}

struct task{
	void(*f)(arr_void);
	arr_void arg;
};

void invoke_impl(void_pfn_voidp task, arr<arr_void> const& segs){
	active= true;

	int workers= segs.size();

	latch.set(workers);
	int i=1;
	for(auto& thread : threads)
		thread.order(task, &segs[i++], &latch);
	task(&segs[0]);//run task on invoking thread
	latch.tick();
	latch.await();

	active= false;
}

template<typename T>
void segment(arr<T> const jobs, vector<arr<T>>& ret){
	int const denom= ret.size();
	assert(denom>0);

	size_t total= jobs.size();
	size_t span= total/denom;
	size_t rem= total - span*denom;

	for(int i=0; i!=denom; i++){
		arr<T>& s= ret[i];
		s.base= jobs.base + i*span;
		s.stop= s.base + span;
	}

	//give remainder to last thread
	ret[denom-1].stop+= rem;
}


}


#include "execq.hpp"

void execq::push(exec* e){
	mut.lock();
	q<<e;
	mut.unlock();
}
void execq::flush(){
	vector<exec*> qc;
	mut.lock();
	if(!q.empty())
		swap(q, qc);//create a copy to minimize blocking
	mut.unlock();

	for(auto i:qc){
		i->invoke();
		delete i;
	}
}
}