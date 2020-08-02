#include "string.hpp"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include "math.hpp"
namespace doot{

constexpr int STRMAX= 0x100;


str& str::fmt(char const* fmt, ...){
	va_list vargs0;
	va_start(vargs0,fmt);
	va_list vargs1;
	va_copy(vargs1,vargs0);
	//these are mutated by printf

	size_t l= vsnprintf(0,0,fmt,vargs0)+1;
	size_t c= dat.capacity();
	size_t s= dat.size();
	size_t n= s+l;
	if(n>c)
		dat.realloc(n);
	dat.stop--;
	vsnprintf(dat.stop,l,fmt,vargs1);
	dat.stop+= l;

	retthis;
};



str& str::operator+=(char const* c){
	size_t l= strnlen_s(c,STRMAX);
	if(l>=STRMAX){
		bad("str::operator=(char const* c) aborting null-terminator search, enormous str, probably corrupt");
		retthis;
	}
	dat.stop--;//remove terminator
	forcount(i,l){
		char ci= c[i];
		dat.make(ci);
	}
	dat.make(0);//readd terminator
	retthis;
}
str& str::operator+=(str const& that){
	dat.stop--;//remove terminator
	dat.make(that.dat);
	retthis;
}

bool str::operator==(str const& that) const{
	if(dat.base==that.dat.base)
		return true;
	if(size()!=that.size())
		return false;

	char const* a= dat.base;
	char const* b= that.dat.base;
	forcount(i, size()){
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