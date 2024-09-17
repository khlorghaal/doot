#pragma once
#include "primitives.hpp"
#include "list.hpp"
#include "hmap.hpp"

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
	str(cstr    b):str(){ cat(b); }
	str(str cre b):str(){ dat.addl(b.dat); }
	//str(str &&   b){ acquire(dat,b.dat); }
	str& op=(str cre  b){ dat.clear(); dat.addl(b.dat); reth; }
	str& op=(str &&   b){ free(dat); acquire(dat,b.dat); reth; }
	str& op=(cstr b){ op=(str(b)); reth; }
	static str itod(u32 x); static str itod(i32 x);		static str itox(u32 x); static str itox(i32 x);
	static str itod(u64 x); static str itod(i64 x);		static str itox(u64 x); static str itox(i64 x);
	static str ftod(f64 x);                        		static str ftox(f64 x);
	str(i64 x):str(itod(x)){};
	str(u32 x):str(itod(x)){};
	str(u64 x):str(itod(x)){};
	str(f64 x):str(ftod(x)){};
	//str(rati b) "[%i/%i]",b.num,b.den
	tple str(E* e) :str(itox( (u64)(void*)e )){};

	tple str(list<E>&);
	tpl<typn K, typn V> str(hmap<K,V>&);

    tpl<typn... E> str( E&&... e)
    	:str(     (str((E&&   )e)+...)){}//i hate packs
	~str()= default;

	void clear(){ dat.clear(); }

	str& cat(str cst& b){ dat.addl(b.dat); reth; };
	str& cat(cstr     b);
	tple str& op =(E&& b){ clear(); re cat(b); }
	tple str& op+=(E&& b){          re cat(b); }
	     str  op+ (str cre b){ str r= *this; r+=b; retr;};
	
	bool op!() cst { re !dat.base || dat.size()==0; };
	bool op==(str cre  ) cst;
	bool op==(cstr    b) cst{ re op==(str(b));};

	op cstr(){
		ass(dat.base);
		dat.prealloc(dat.size()+1);
		*dat.stop= 0;//add null without affecting object
		re dat.base; }

	siz size() cst{ re dat.size(); }
};

hash_t hash(str cre str){
	//djb2
	unsigned int r= 5381;
	EACH(c,str.dat)
		r= r+(r<<5)+c;
	retr;
}

inl str& str::cat(cstr b){
	cstr c= b;
	while(*c++){
		if(unlikely((c-b)>TOO_BIG)){
			warn("cstr termin't, omitted");
			reth;
		}
	}
	dat.addl(arr<char const>{b,c-1});
	reth;
};

#define o(a) op+=(a)
tple str::str(list<E>& l):str(){
	EACH(e,l){
		o(e); o(",");}
};
tpl<typn K, typn V> str::str(hmap<K,V>& h):str(){
	list<pair<K,V>> a;
	h.key_values_cpy(a);//slow but so is console
	EACH(e,a){
		o("[");	o(e.a); o(":"); o(e.b); o("],");}
		//i hunger for fstrings
};
#undef o

bool str::operator==(str cre that) const{
	re op==<char>((arr<char>)dat, (arr<char>)that.dat);
}


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

}