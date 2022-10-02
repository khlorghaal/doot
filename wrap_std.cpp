//encloses all std+stl headers to prevent name collision

#include <stdio.h>
#include <string.h>

#include <threads.h>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <unordered_map>


namespace doot{

extern void err(char const*);

#define OPAQIMPL_CDTOR(T) \
void T##_##CTOR(void*& p){ p= new  _##T(); };\
void T##_##DTOR(void*& p){ delete (_##T*)p; };
#define OPAQIMPL_F(T,F) \
void T##_##F(void* p){ ((_##T*)p)->F(); }

struct _mutex:std::mutex{};

struct _lock{
	_mutex mut;
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


struct _latch{
	_mutex mut;
	_lock lck;
	volatile int count= 0;
	void set(int count){
		mut.lock();
		if(count!=0)
			err("latch:set:impl: count!=0");
		count= count;
		mut.unlock();
	}
	void tick(){
		mut.lock();
		if(count>0)
			count--;
		if(count==0)
			lck.wake();
		mut.unlock();
	}
	void wait(){
		lck.wait();
	}
};


OPAQIMPL_CDTOR(mutex);
OPAQIMPL_CDTOR(lock);
OPAQIMPL_CDTOR(latch);

OPAQIMPL_F(mutex,lock);
OPAQIMPL_F(mutex,unlock);

OPAQIMPL_F(lock,wait);
OPAQIMPL_F(lock,wake);

void latch_set(void* p, int count){
	((_latch*)p)->set(count);
};
OPAQIMPL_F(latch,tick);
OPAQIMPL_F(latch,wait);




int vcore_count(){
	return 8;
	//fixme hack
	//query the os
}

//(void*->void)->(void*->int(0))
std::mutex _thr_mut;
static std::unordered_map<void*,void(*)(void*)> _fntr_vp2i_map;
int _fntr_vp2i(void* p){
	_thr_mut.lock();
	auto f= (_fntr_vp2i_map[p]);
	_thr_mut.unlock();
	f(p);
	return 0;
}
void  _thread(char const* name, void(*f)(void*),void* arg){
	thrd_t thr;
	_thr_mut.lock();
	_fntr_vp2i_map.emplace(arg,f);
	_thr_mut.unlock();
	thrd_create(&thr,&_fntr_vp2i,arg);
};

void* _malloc(size_t s){
	return ::malloc( s); };
void  _free(   void* p){
	::free(   p); };
void* _realloc(void* p, size_t s){
	return ::realloc(p,s); };
void _memcpy(void* dst, void* src, size_t len){
	::memcpy(dst,src,len); }

size_t _vsnprintf( char * buf, size_t bufsz,
               const char * fmt, va_list vlist){
	return vsnprintf(buf,bufsz,fmt,vlist);
}
size_t _strnlen(char const* s, size_t n){
	return strnlen(s,n);
}


static std::mutex console_mut;
void _printf(char const* s){
	console_mut.lock();
	printf("%s",s);
	console_mut.unlock();
}
};
