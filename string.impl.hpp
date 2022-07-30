#include <stdarg.h>

#include "string.hpp"

namespace doot{

constexpr int STRMAX= 0x1000;

//sdtdio has count defined, macro interference
extern sizt _vsnprintf( char * buf, size_t bufsz,
               const char * fmt, va_list vlist);

extern sizt _strnlen(char const*,sizt);

str str::fmt(char const* fmt, ...){
	str ret;
	BIND(ret,dat);

	va_list vargs;
	va_start(vargs,fmt);
	va_list vargc;
	va_copy(vargc,vargs);
	//these are rudely mutated by printf

	sizt l= _vsnprintf(0,0,fmt,vargs)+1;
	sizt c= dat.capacity();
	sizt s= dat.size();
	sizt n= s+l;
	if(n>c)
		dat.realloc(n);
	dat.stop--;
	_vsnprintf(dat.stop,l,fmt,vargc);
	dat.stop+= l;

	va_end(vargs);
	va_end(vargc);

	return ret;
};
str& str::fmtcat(char const* fmt,...){
	va_list vargs;
	va_start(vargs,fmt);
	va_list vargc;
	va_copy(vargc,vargs);
	//these are rudely mutated by printf

	sizt l= _vsnprintf(0,0,fmt,vargs)+1;
	sizt c= dat.capacity();
	sizt s= dat.size();
	sizt n= s+l;
	if(n>c)
		dat.realloc(n);
	dat.stop--;
	_vsnprintf(dat.stop,l,fmt,vargc);
	dat.stop+= l;

	va_end(vargs);
	va_end(vargc);

	retthis;
}

str& str::cat(char const* _c){
	char* c= const_cast<char*>(_c);
	//jesus fuck
	sizt l= _strnlen(c,STRMAX);
	if(l>=STRMAX){
		bad("str::cat(char const* c) donked null-terminator search, corrupt andor bigly");
		retthis;
	}
	sizt s= dat.size();
	dat.realloc_greed(s+l);
	char* d= dat.stop;
	copy(
		arr<char>{d-1,d+l  },// -1 as [stop-1]==terminator; overwrite it
		arr<char>{c  ,c+l+1});//terminator copied from that
	dat.stop+= l;
	return *this;
}
str& str::cat(str const& b){
	sizt l= b.dat.size();
	char* c= const_cast<char*>(b.dat.base);
	if(l>=STRMAX){
		bad("str::cat(str const&) too bigly");
		retthis;
	}
	sizt s= dat.size();
	dat.realloc_greed(sizt(s+l));
	char* d= dat.stop-1;
	copy(
		arr<char>{d,d+l},
		arr<char>{c,c+l});
	dat.stop+= l;
	return *this;
}

bool str::operator==(str const& that) const{
	if(dat.base==that.dat.base)
		return true;
	if(size()!=that.size())
		return false;
 
	char const* a= dat.base;
	char const* b= that.dat.base;
	auto n= size();
	RA(i,n){
		if(a[i]!=b[i])
			return false;
	}
	return true;
}


hash_t hash(str const& str){
	arr<char> s= {
		const_cast<char*>(str.dat.base),
		const_cast<char*>(str.dat.stop)
	};
	//djb2
	unsigned int x= 5381;
	EACH(c,s)
		x= (x<<5)+x+c;
	return x;
}

}

#undef op
