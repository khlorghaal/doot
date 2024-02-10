//encloses all std+stl headers to prevent name collision
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <threads.h>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <unordered_map>

#define _DOOT_NO_NEW_ 1
#include "doot.hpp"//opaque macros
#include "thread.hpp"

#include <chrono>
#include <thread>
//FIXME PURGE STD
using namespace std::this_thread;
using namespace std::chrono;

namespace doot{

extern void _err(char const*, char const*);

struct _mutex:std::mutex{};
//i dont remember why this isnt a typedef
//there is probably a reason for that

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
	std::atomic<int> count= 0;
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



void mutex_CTOR(void* s){
	OPAQ_M_CTOR(s,_mutex);
}
void mutex_DTOR(void* s){
	OPAQ_M_DTOR(s,_mutex);
}
void mutex_lock(void* s){
	OPAQ_T_DEREF(s,_mutex,m);
	m.lock();
}
void mutex_locknt(void* s){
	OPAQ_T_DEREF(s,_mutex,m);
	m.unlock();
}

void lock_CTOR(void* s){
	OPAQ_M_CTOR(s,_lock);
}
void lock_DTOR(void* s){
	OPAQ_M_DTOR(s,_lock);
}
void lock_wait(void* s){
	OPAQ_T_DEREF(s,_lock,l);
	l.wait();
}
void lock_wake(void* s){
	OPAQ_T_DEREF(s,_lock,l);
	l.wake();
}

void latch_CTOR(void* s){
	OPAQ_M_CTOR(s,_latch);
}
void latch_DTOR(void* s){
	OPAQ_M_DTOR(s,_latch);
}

void latch_set(void* s, int i){
	OPAQ_T_DEREF(s,_latch,l);
	l.set(i);
}
void latch_tick(void* s){
	OPAQ_T_DEREF(s,_latch,l);
	l.tick();
}
void latch_wait(void* s){
	OPAQ_T_DEREF(s,_latch,l);
	l.tick();
}






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

void sleep(nsec t){
	std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

void* _malloc(siz s){
	return ::malloc( s); };
void  _free(   void* p){
	::free(   p); };
void* _realloc(void* p, siz s){
	return ::realloc(p,s); };
void _memcpy(void* dst, void* src, siz len){
	::memcpy(dst,src,len); }
void _memclr(void* dst, siz len){
	::memset(dst,0,len); }

size_t _vsnprintf( char * buf, siz bufsz,
               const char * fmt, va_list vlist){
	return vsnprintf(buf,bufsz,fmt,vlist);
}
size_t _strnlen(char const* s, siz n){
	return strnlen(s,n);
}

char const* strfmt_cstr(char const* s, ...){
	va_list args;
	va_start(args,s);
    auto ret= strfmt(s,args);
    va_end(args);
    retret;
}

struct console_stream{
	decltype(stdout) self;
	static console_stream _stdout;
	static console_stream _stderr;
};
console_stream console_stream::_stdout= {stdout};
console_stream console_stream::_stderr= {stderr};
console cout= {&console_stream::_stdout};
console cerr= {&console_stream::_stderr};
std::mutex console_mut= std::mutex();
console& console::operator()(str const& x){
	console_mut.lock();
	fprintf(stream->self,"%s\n",(cstr)x);
	console_mut.unlock();
	rett;
};


};
