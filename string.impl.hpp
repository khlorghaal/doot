#include <stdarg.h>

#include "string.hpp"

namespace doot{
using size_t= siz;

constexpr int STRMAX= 0x1000;

//sdtdio has count defined, macro interference
extern sizt _vsnprintf( char * buf, size_t bufsz,
               const char * fmt, va_list vlist);

extern sizt _strnlen(char const*,sizt);

str str::fmt(char const* fmt, ...){
	str ret;
	usef(ret,dat);

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

	va_end(vargc);
	va_end(vargs);

	retret;
};

bool str::operator==(str const& that) const{
	if(dat.base==that.dat.base)
		return true;
	if(size()!=that.size())
		return false;
 
	arr<char> a= dat;
	arr<char> b= that.dat;
	auto n= size();
	ZIP(x,y,a,b)
		if(unlikely(x!=y))
			return false;
	return true;
}


hash_t hash(str const& str){
	//djb2
	unsigned int r= 5381;
	EACH(c,str.dat)
		r= r+(r<<5)+c;
	retr;
}

}

#undef op
