#include "string.hpp"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include "math.hpp"
namespace doot{

constexpr int STRMAX= 0x1000;

str str::fmt(char const* fmt, ...){
	str ret;
	BIND(ret,dat);

	va_list vargs;
	va_start(vargs,fmt);
	va_list vargc;
	va_copy(vargc,vargs);
	//these are rudely mutated by printf

	size_t l= vsnprintf(0,0,fmt,vargs)+1;
	size_t c= dat.capacity();
	size_t s= dat.size();
	size_t n= s+l;
	if(n>c)
		dat.realloc(n);
	dat.stop--;
	vsnprintf(dat.stop,l,fmt,vargc);
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

	size_t l= vsnprintf(0,0,fmt,vargs)+1;
	size_t c= dat.capacity();
	size_t s= dat.size();
	size_t n= s+l;
	if(n>c)
		dat.realloc(n);
	dat.stop--;
	vsnprintf(dat.stop,l,fmt,vargc);
	dat.stop+= l;

	va_end(vargs);
	va_end(vargc);

	retthis;
}


str& str::cat(char const* c){
	size_t l= strnlen_s(c,STRMAX);
	if(l>=STRMAX){
		bad("str::operator=(char const* c) aborting null-terminator search, enormous str, probably corrupt");
		retthis;
	}
	dat.stop--;//remove terminator
	forct(l){
		char ci= c[i];
		dat.make(ci);
	}
	dat.make(0);
	retthis;
}

bool str::operator==(str const& that) const{
	if(dat.base==that.dat.base)
		return true;
	if(size()!=that.size())
		return false;

	char const* a= dat.base;
	char const* b= that.dat.base;
	forct(size()){
		if(a[i]!=b[i])
			return false;
	}

	return true;
}


hash_t hash(str const& str){
	auto s= str.cstr();
	//djb2
	unsigned int x= 5381;
	char c;
	while(!!(c=*s++))
		x= (x<<5)+x+c;
	return x;
}

}