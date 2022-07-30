//encloses all std+stl headers to prevent name collision

#include <stdio.h>
#include <string.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>


namespace doot{



struct _mutex:std::mutex{};
OPAQUE_CDTOR_DEFAULT(struct mutex,_mutex)
void mutex::lock(){  (_mutex*)_->std::mutex::lock();   };
void mutex::locknt(){(_mutex*)_->std::mutex::unlock(); };


struct _lock{
	doot::mutex mut;
	std::condition_variable cv;
	volatile bool pred= false;
	void wait(){
		std::unique_lock<std::mutex> lck(mut);
		pred= false;
		cv.wait(lck, [&]{return pred;});
	}
	void wake(){
		mut.lock();
		pred= true;
		cv.notify_all();
		mut.unlock();
	}
};
OPAQUE_CDTOR_DEFAULT(lock,_lock)
void lock::wait(){ (_lock*)_->wait(); };
void lock::wake(){ (_lock*)_->wake(); };


struct _latch{
	doot::mutex mut;
	doot::lock lck;
	volatile int count= 0;
	void  set(int count){
		mut.lock();
		ass(count==0);
		count= count;
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
	void wait(){
		lck.await();
	}
};
OPAQUE_SCDTOR_DEFAULT(latch,_latch)
void latch::set(int count){ (_latch*)_->set(count); };
void latch::tick(        ){ (_latch*)_->tick(    ); };
void latch::wait(        ){ (_latch*)_->wait(    ); };




int vcore_count(){
	return 8;
	//fixme hack
	//query the os
}
};











//used rarely, thus declared extern ad hoc
void  __thread(char const*, void(*)(void*),void*);
void* __malloc(  sizt s){	     	return ::malloc( s); };
void  __free(   void* p){				   ::free(   p); };
void* __realloc(void* p, sizt s){	return ::realloc(p,s); };

void __memcpy(void* dst, void* src, sizt len){
	ass(!!dst&!!src);
	::memcpy(dst,src,len);
}

sizt _vsnprintf( char * buf, size_t bufsz,
               const char * fmt, va_list vlist){
	vsnprintf(buf,bufsz,fmt,vlist);
}
sizt _strnlen(char const* s, size_t n){
	strnlen(s,n);
}


static doot::mutex console_mut;
_printf(char const* s){
	console_mut.lock();
	printf(s);
	console_mut.locknt();
}

}