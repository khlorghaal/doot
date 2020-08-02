#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "thread.hpp"

namespace doot{

int vcore_count();
#ifdef _WIN32
int vcore_count(){
	return 8;//TODO HACK LOL
};
#elif LINUX
#endif

vector<std::thread*> threads;
void thread(str& name_, fpvvp f, void* arg){
	threads.make( new std::thread(f,arg) );
	//FIXME leak
	//TODO make warps into objects which can be properly lifetimed
	//low priority because rarely will warps need destruction other than termination
}

struct _mutex{ std::mutex _; };
mutex::mutex(){	_= new _mutex(); };
mutex::~mutex(){ delete _; }
void mutex::lock(){ _->_.lock(); }
void mutex::locknt(){ _->_.unlock(); }




struct _lock{
	std::mutex mut;
	std::condition_variable cv;
	volatile bool pred= false;

	void await(){
		std::unique_lock<std::mutex> lck(mut);
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
lock::lock(){ _= new _lock(); }
lock::~lock(){ delete _; }
void lock::await(){ _->await(); };
void lock::wake(){ _->wake(); };


struct _latch{
	mutex mut;
	lock lck;
	volatile int count= 0;

	void set(int count_){
		mut.lock();
		ass(count==0);
		count= count_;
		mut.locknt();
	}
	void tick(){
		mut.lock();
		if(count>0)
			count--;
		if(count==0)
			lck.wake();
		mut.locknt();
	}
	void await(){
		lck.await();
	}
};
latch::latch(){ _= new _latch(); }
latch::~latch(){ delete _; }
void latch::set(int count){ _->set(count); }
void latch::tick(){ _->tick(); }
void latch::await(){ _->await(); }



namespace tasker{

int poolsize;//excludes invoking worker
volatile bool active= false;

latch ltch;
vector<executor> threads;

void init(){
	poolsize= vcore_count();
	
	if(poolsize>2)//more than 2 vcores
		poolsize--;//leave one free
	if(poolsize>0)
		poolsize--;//invoker runs a task on itself
	ass(poolsize>=0);

	threads.realloc(poolsize);

	int i=0;
	for(auto& thread : threads)
		new (&thread) executor(str("tasker")+=i++);
}

void _invoke(fpvvp f, arr<void*> segs){
	active= true;

	int workers= segs.size();

	ltch.set(workers);
	int i=1;
	for(auto& thread : threads)
		thread.order(f, &segs[i++], &ltch);
	f(&segs[0]);//run task on invoking thread
	ltch.tick();
	ltch.await();

	active= false;
}

}

}