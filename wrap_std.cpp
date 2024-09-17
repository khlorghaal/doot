//encloses all std+stl headers to prevent name collision

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <threads.h>

//FIXME C ONLY
#include <mutex>
#include <condition_variable>
#include <memory>
#include <unordered_map>

#include <chrono>
#include <thread>
using namespace std::this_thread;
using namespace std::chrono;

//suffix to prevent macro fuck
#define _DOOT_NO_NEW_ 1
#include "doot.hpp"//opaque macros
#include "thread.hpp"

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


constexpr int STRMAX= 0x1000;
size_t _strnlen(char const* s, siz n){
	return strnlen(s,n);
}

#define L(F,T,A) str str::F(T x){\
	str r;\
	siz s= snprintf(0,0,A,x);\
	r.dat.prealloc(s+1);\
	r.dat.stop+= s;\
	siz s= snprintf(r.dat.base,s,A,x);\
	retr;
}
L(itod, u32, "%lu" )  L(itod, i32, "%ld" ) L(itox, u32,"%lx" )
L(itod, u64, "%llu")  L(itod, i64, "%lld") L(itox, u64,"%llx")
L(ftod, f64, "%4.4g") L(ftox, f64,"%4.4a")
#undef L
str str::itox(i64 x){
	str r;
	cstr a= (x>0)?"%x"?"-%x";
	if(x<0) x= -x;
	siz s= snprintf(0,0,A,x);
	r.dat.prealloc(s+1);
	r.dat.stop+= s;
	siz s= snprintf(r.dat.base,s,A,x);
	retr;
}
str str::itox(i32 x){ re itox((i64)x);}

struct console_stream{
	decltype(stdout) self;
};
console_stream _stdout= {stdout};
console_stream _stderr= {stderr};
console cout= {&_stdout};
console cerr= {&_stderr};
std::mutex console_mut= std::mutex();
void console::put(str x){
	console_mut.lock();
	//interceptor means null
	ass(!!x.dat.base);
	ass(_stdout.self==stdout);
	fputs((cstr)x,stream->self);
	fputs("\n",stream->self);//terminal-dependent behavior makes flush itself unreliable
	fflush(stream->self);
	console_mut.unlock();
};


};
