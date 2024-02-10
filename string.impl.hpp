#include <stdarg.h>

#include "string.hpp"

namespace doot{
using size_t= siz;

constexpr int STRMAX= 0x1000;

extern sizt _vsnprintf( char * buf, size_t bufsz,
               const char * fmt, va_list vlist);

extern sizt _strnlen(char const*,sizt);

str str::fmt(char const* fmt, ...){
	str ret;
	usem(ret,dat);

	va_list vargs;
	va_start(vargs,fmt);
	va_list vargc;
	va_copy(vargc,vargs);
	//these are rudely mutated by printf

	sizt l= _vsnprintf(0,0,fmt,vargs)+1;
	dat.realloc(l);
	dat.stop= dat.base+l;
	_vsnprintf(dat.base,l,fmt,vargc);

	va_end(vargc);
	va_end(vargs);

	retret;
};

bool str::operator==(str const& that) const{
	if(dat==that.dat)
		re 1;
	ZIP(a,b,dat,that.dat){
		if(unlikely(a!=b))
			re 0;
	}
	re 1;
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
