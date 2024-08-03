#pragma once
#include "primitives.hpp"
#include "list.hpp"
#include "hmap.hpp"

namespace doot{

/*
null terminated for c friendness, but should be treated as arrays
advanced string functions are wrapped/socketed std

quite unoptimized
doot neednt good strings
*/
struct str{
	lis<char> dat;//includes terminator

	str(){ clear(); };
	str(cstr b):str(){ cat(fmt(b)); }
	str(str cre b):str(){ dat.addl(b.dat); }
	//str(str &&   b){ waive(dat,b.dat); }
	str& op=(str cre b){ dat.clear(); dat.addl(b.dat); reth; }
	//str& op=(str &&   b){ free(dat); waive(dat,b.dat); reth; }
	str& op=(cstr b){ op=(str(b)); reth; }
	str(i32 x):str(){ cat(fmt("%d",   x)); }
	str(u32 x):str(){ cat(fmt("%u",   x)); }
	str(i64 x):str(){ cat(fmt("%lld", x)); }
	str(u64 x):str(){ cat(fmt("%llu", x)); }
	str(f32 x):str(){ cat(fmt("%3.3f",x)); }
	str(f64 x):str(){ cat(fmt("%6.4f",x)); }
	tple str(list<E>&);
	tpl<typn K, typn V> str(hmap<K,V>&);
	//str(rati b){fmt(*this,"[%i/%i]",b.num,b.den);}
	//str(arr<char> a){ fmt("%s",a.base); rethis; }
	static str fmt(char const* fmt,...);//constructor
	~str()= default;

	void clear(){
		dat.clear();
		dat.add(0);
	}

	str& cat(str cst& b){ dat.stop--; dat.addl(b.dat); reth; };
	tple str& op =(E b){ clear(); return cat(str(b)); }
	tple str& op+=(E b){          return cat(str(b)); }
	str op+(str cst& b){ str r= *this; r+=b; retr;};
	
	bool op!() const { return !dat.base || dat.size()<=1; };
	bool op==( str const&  ) const;
	bool op==(char const* b) const{ return op==(str(b)); };
	bool op!=( str const& b){ return !(op==(b));};
	bool op!=(char const* b){ return !(op==(b));};

	op cstr() const{ return dat.base; }

	siz size() const{ siz r= dat.size(); ass(r>=0); retr-1; }//because null terminator
};

#define strfmt str::fmt

hash_t hash(str const& s);

struct console_stream;
struct console{
	console_stream* stream;
	console& op()(str const&);
	//tple console& op()(E e){ op()(str((E)e)); rethis; };
	//todo strfmt forward
};
extern console cout;
extern console cerr;


#define o(a) op+=(str(a))
tple str::str(list<E>& l):str(){
	EACH(e,l){
		o(e); o(",");
	}
};
tpl<typn K, typn V> str::str(hmap<K,V>& h):str(){
	list<pair<K,V>> a;
	h.key_values_cpy(a);
	EACH(e,a){
		o("[");	o(e.a); o(","); o(e.b); o("],");
	}//i hunger for fstrings
};
#undef o


}