#pragma once
#include "vector.hpp"

namespace doot{

//null terminated for c friendness
struct str{
	vector<char> dat;
	char const* cstr() const{ return dat.base; }

	str(){ dat.make(0); };
	str(str const& b){ dat.make(b.dat); };
	str(str&&      b){ dat.make(b.dat); };//opt 
	TPLE str(E const& b):str(){ op+=(b); }
	~str()= default;

	void clear(){
		dat.clear();
		dat.make(0);
	}

	str& op= (str const& b){ clear(); return op+=(b);}
	str& op= (str&&      b){ clear(); return op+=((str&&)b);}

	str& op+=(char const*);
	str& op+=(str const&);
	str& op+=(str&& b){ return op+=((str const&)b); };
	#define L(T, x) \
	str& op+=(T b){ fmt(x,b); retthis; }
	L(i32,"%d");
	L(ui32,"%u");
	L(i64,"%dll");
	L(ui64,"%ull");
	L(f32,"%3.4f");
	L(f64,"%6.4f");
	#define L(A,B) \
	str& op+=(A b){ return op+=((B)b); }
	//L(f32,f64);
	//L(i32,i64);
	//L(ui32,ui64);
	#undef L
	//str& op+=(rati b){fmt(*this,"[%i/%i]",b.num,b.den);}


	str& op+=(arr<char> a){ dat.make(a); retthis; }

	bool op!() const { return !dat.base || dat.size()<=1; };
	bool op==(str const&) const;
	bool op==(char const* b) const{ return op==(str(b)); };

	op char const*(){ return cstr(); }

	size_t size() const{ return dat.size()-1; }//because null terminator

	str& fmt(char const* fmt,...);//adds
};

unsigned int hash(str const& s);




struct console{
	console& op()(str const&);
	TPLE console& op()(E e){ op()(str((E)e)); retthis; };
};
extern console cout;
extern console cerr;

}