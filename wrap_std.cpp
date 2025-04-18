//encloses all std+stl headers to prevent name collision

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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


void sleep(nsec t){
	std::this_thread::sleep_for(std::chrono::nanoseconds(t));
}

struct _mutex:std::mutex{
	void locknt(){ std::mutex::unlock(); }//lol
};


struct _lock{
	_mutex mut;
	std::condition_variable cv;
	bool awaken= false;
	bool waiting= false;

    void wait() {
        std::unique_lock<std::mutex> lck(mut);
        waiting= true;
        awaken= false;
        cv.notify_all();
        cv.wait(lck, [&]{ re awaken; });
    }
    void wake(bool blocking) {
        std::unique_lock<std::mutex> lck(mut);
        if(blocking)//block if no threads waiting until one is
        	cv.wait(lck, [&]{ re waiting; });
        waiting= false;
        awaken= true;
        cv.notify_all();
    }
};


struct _latch{
	_mutex mut;
	//_lock lck; dont reuse lock
	//the cv requires the single mut + count as predicate
	std::condition_variable cv;
	int count= 0;
	void set(int count_){
        mut.lock();
		if(count>0){//allow negative to allow underflow
			warn("latch:set:impl: count>0");
			cv.notify_all();
		}
		count= count_;
		mut.locknt();
	}
	void tick(){
	    mut.lock();
	    if(--count<=0)
	    	cv.notify_all();
	    if(count<0)
	    	warn("latch underflow");
	    mut.locknt();
	}
	void wait(){
        std::unique_lock<std::mutex> lck(mut);
        cv.wait(lck, [&]{ re count<=0; });
	}
};


//f(void) != f()
OPAQ_CDTR(mutex);
OPAQ_MTHD(mutex,lock  , NOTHING,NOTHING, void);
OPAQ_MTHD(mutex,locknt, NOTHING,NOTHING, void);

OPAQ_CDTR(lock);
OPAQ_MTHD(lock,wait, NOTHING,NOTHING, void);
OPAQ_MTHD(lock,wake, bool,_         , void);

OPAQ_CDTR(latch);
OPAQ_MTHD(latch,set , int,_          ,void);
OPAQ_MTHD(latch,tick, NOTHING,NOTHING,void);
OPAQ_MTHD(latch,wait, NOTHING,NOTHING,void);


int vcore_count(){
	return std::thread::hardware_concurrency();
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
void  _thread(cstr name, void(*f)(void*),void* arg){
	thrd_t thr;
	_thr_mut.lock();
	_fntr_vp2i_map.emplace(arg,f);
	_thr_mut.unlock();
	thrd_create(&thr,&_fntr_vp2i,arg);
};


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
void  _memset( void* dst, u8 v, sizt len){
	::memset(dst,v,len); }

constexpr int STRMAX= 0x1000;
size_t _strnlen(char const* s, siz n){
	return strnlen(s,n);
}

#define L(F,T,A) str str::F(T x){\
	int s= snprintf(0,0,A,x);\
	if(s<0){ warn("prin'tf"); re ""; }\
	str r; r.dat.prealloc(s+1);\
	r.dat.stop+= s;\
	snprintf(r.dat.base,s+1,A,x);\
	retr;}
L(itod, u32, "%u" )  L(itod, i32, "%d"  ) L(itox, u32,"%x"  )
L(itod, u64, "%llu") L(itod, i64, "%lld") L(itox, u64,"%llx")
L(ftod, f64, "%.4f") L(ftox, f64, "%.4f")
#undef L
str str::itox(i64 x){
	str r;
	cstr a= (x>0)?"%x":"-%x";
	if(x<0) x= -x;
	int s= snprintf(0,0,a,x);
	r.dat.prealloc(s+1);
	r.dat.stop+= s;
	snprintf(r.dat.base,s,a,x);
	retr;}
str str::itox(i32 x){ re itox((i64)x);}

struct console_stream{
	decltype(stdout) self;
	std::mutex mut;
};
console_stream _stdout= {stdout};
console_stream _stderr= {stderr};
console cout= {&_stdout};
console cerr= {&_stderr};
void console::put(str x){
	ass(!!x.dat.base);
	ass(!!stream);
	stream->mut.lock();
	//interceptor means null
	fputs((cstr)x,stream->self);
	fputs("\n",   stream->self);//terminal-dependent behavior makes flush itself unreliable
	stream->mut.unlock();
	fflush(       stream->self);
};

bool file_dump(list<u8>& ret, str name){
	print("fdump\""+name+"\"");
	errno= 0;
	//errno must manual reset
	//glfw et al may set after success
	FILE* file= fopen(name, "r");
	if(!file)
		re true;
	if(!!errno){
		warn(name+" file errno "+strerror(errno));
		fclose(file);
		re true;
	}
    fseek(file, 0, SEEK_END);
    siz size = ftell(file);
    fseek(file, 0, SEEK_SET);
    ret.prealloc(ret.size()+size);
    if(fread(ret.stop, 1, size, file) != size){
        fclose(file);
        re true;
    }
    ret.stop+= size;
    fclose(file);
    re false;
}
bool file_dump(str& ret, str name){
	ass(sizeof(ret.dat.base[0])==sizeof(u8));//no wchar support
	re file_dump(rcas<list<u8>>(ret),name);
}


};
