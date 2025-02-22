#pragma once
#include "primitives.hpp"
#include "list.hpp"
#include "hmap.hpp"
#include "idheap.hpp"

namespace doot{

/*
null terminated for c friendness, but should be treated as arrays
advanced string functions are wrapped/socketed std

rather unoptimized
doot neednt good strings, use something else if you do
*/
struct str{
	lis<char> dat;//no terminator because skill issues

	str()= default;
	str(cstr    b):dat(){ cat(b); }
	str(str cre b):dat(){  dat.addl(b.dat); }
	str(str &&  b):dat((lis<char>&&)b.dat){}
	str(i64 x):str(itod(x)){};//to prevent cast to cstr
	str(u64 x):str(itod(x)){};
	tple str(E* e) :str(itox( (u64)(void*)e )){};
	tple explicit str(E&& x):dat(){ cat((E&&)x); };
	str& op=(str cre  b){ dat.clear();  dat.addl(b.dat); reth; }
	str& op=(str &&   b){ free(dat); acquire(dat,b.dat); reth; }
	str& op=(cstr b){ op=(str(b)); reth; }
	static str itod(u32 x); static str itod(i32 x);		static str itox(u32 x); static str itox(i32 x);
	static str itod(u64 x); static str itod(i64 x);		static str itox(u64 x); static str itox(i64 x);
	static str ftod(f64 x);                        		static str ftox(f64 x);
	//cat forces concretion
	void cat(str cre b){ dat.addl(b.dat); };
	void cat(str &&  b){ dat.addl(b.dat); };
	void cat(cstr    b);
	void cat(i32 x){ cat(itod(x)); };
	void cat(i64 x){ cat(itod(x)); };
	void cat(u32 x){ cat(itod(x)); };
	void cat(u64 x){ cat(itod(x)); };
	void cat(f32 x){ cat(ftod(x)); };
	void cat(f64 x){ cat(ftod(x)); };
	void cat(long x         ){ re cat(itod((i64)x)); };
	void cat(unsigned long x){ re cat(itod((u64)x)); };
	//str(rati b) "[%i/%i]",b.num,b.den
	     str  op+ (str cre b){ str r= *this; r.cat(       b); retr; }
	     str  op+ (str &&  b){ str r= *this; r.cat((str&&)b); retr; }
	tple str  op+ (E&& b){     str r= *this; r.cat((  E&&)b); retr; }
	tple str& op =(E&& b){ clear(); cat((  E&&)b);  reth;}
	tple str& op+=(E&& b){          cat(   str(b)); reth;}

	//i gave up on variads via various cursed approaches they all suck

	~str()= default;

	void clear(){ dat.clear(); }

	
	bool op!() cst { re !dat.base || dat.size()==0; };
	bool op==(str cre that) cst{
		re ((arr<char>)dat)==((arr<char>)that.dat);}
	bool op==(cstr b) cst{ re op==(str(b) );};

	op cstr(){
		ass(dat.base);
		dat.prealloc(dat.size()+1);
		*dat.stop= 0;//add null without affecting object
		re dat.base; }

	siz size() cst{ re dat.size(); }


	tple str(arr<E> cre l){
		each(e,l){
			cat(e);cat(",");}
		cat(";");
	};
	tple str(lis<E> cre l){
		each(e,l){
			cat(e);cat(",");}
		cat(";");
	};
	tpl<typn K, typn V> str(hmap<K,V> cre h){
		each_hmap(k,v,h){
			cat(k);cat(":");cat(v);cat(",");}//i hunger for fstrings //so true oomfie
		cat(";");
	};
	tple str(idheap<E> cre h){
		each(e,h.heap)
			cat(e);
		//todo ids
	}

	siz len(){ re dat.len(); }
};

inl hash_t hash(str cre str){
	//djb2
	unsigned int r= 5381;
	each(c,str.dat)
		r= r+(r<<5)+c;
	retr;
}

inl void str::cat(cstr b){
	cstr c= b;
	while(*c++){
		if(unlikely((c-b)>TOO_BIG)){
			warn("cstr termin't, omitted");
			re;
		}
	}
	dat.addl(arr<char const>{b,c-1});
};




inl str operator"" _s(cstr cstr, unsigned long){ re str(cstr); }

struct console_stream;
struct console{
	console_stream* stream;
	void put(str);//coerce
	tples console& op()(E&&... e){
		str s(e...);
		put(s); reth; };
	#define L(V) console& op()(str V e){ put(e); reth; };
		VTYP_RX
	#undef  L
};
extern console cout;
extern console cerr;

#undef warn
#undef bad
#undef err
#undef assert
#define warn(...) _warn((SRCLOC),(cstr)(str()+__VA_ARGS__));
#define bad( ...)  _bad((SRCLOC),(cstr)(str()+__VA_ARGS__));
#define err( ...)  _err((SRCLOC),(cstr)(str()+__VA_ARGS__));

//redef to work with strcats, predefined so string can use them
#ifdef DEBUG
#define assert(X) if(!unlikely(X)){err(str("ass failure: ")+stringize(X));}
#else
	#define assert(X) {}
#endif


}