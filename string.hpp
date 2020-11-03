#pragma once
#include "vector.hpp"

namespace doot{

//null terminated for c friendness
struct str{
	vector<char> dat;//includes terminator
	char const* cstr() const{ return dat.base; }

	str(){ dat.make(0); };
	str(str const&  b){ dat.make(b.dat); };
	str(str const&& b){ dat.make(b.dat); };//opt b mv dtor
	str(char const* b):str(){ cat(b); }
	//tple str(E b){ op=<E>(b); }
	//str(char const* c   ):str(){ op+=(c); }
	~str()= default;

	void clear(){
		dat.clear();
		dat.make(0);
	}

	static str fmt(char const* fmt,...);//construct
	str& fmtcat(char const* fmt,...);//concatenate

	str& cat(arr<char> a){
		dat.stop--;
		dat.make(a);
		dat.make(0);
		retthis;
	}
	str& cat(char const* a);
	str& cat(str const& a){ return cat(a.dat); }

	//template assignors cause fucktons of incomprehensible default generation issues

	#define L(T) str& op=(T b){ clear(); return op+=((T)b); }
		L(char const*);
		L(str const&);
		L(str const&&);
		L(arr<char>);
	#undef L

	#define L(T) str& op+=(T a){ return cat((T)a); };
		L(char const*);
		L(arr<char>);
		L(str const&);
		L(str const&&);
	#undef L
	#define L(T, x) \
		str& op+=(T b){          return fmtcat(x,b); }\
		str& op= (T b){ clear(); return fmtcat(x,b); }
		L( i32,"%d");
		L(ui32,"%u");
		L( i64,"%dll");
		L(ui64,"%ull");
		L( f32,"%3.4f");
		L( f64,"%6.4f");
		//str& op+=(rati b){fmt(*this,"[%i/%i]",b.num,b.den);}
		//str& op+=(arr<char> a){ fmt("%s",a.base); retthis; }
	#undef L
	
	#define L(A,B) \
	FWD_CAST(str&, op+=, A,B);\
	FWD_CAST(str&, op= , A,B);
	//L(f32,f64);
	//L(i32,i64);
	//L(ui32,ui64);
	#undef L

	bool op!() const { return !dat.base || dat.size()<=1; };
	bool op==(str const&) const;
	bool op==(char const* b) const{ return op==(str(b)); };

	op char const*(){ return cstr(); }

	size_t size() const{ return dat.size()-1; }//because null terminator
};

unsigned int hash(str const& s);




struct console{
	console& op()(str const&);
	tple console& op()(E e){ op()(str((E)e)); retthis; };
};
extern console cout;
extern console cerr;

}