#include "string.hpp"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include "math.hpp"
namespace doot{

constexpr int STRFMTMAX= 0x10;//!! FIXME this is set low to test failures

string::string()= default;
string::~string()= default;

string::string(char const*const c){
	size_t len= strlen(c);
	if(len==-1)
		throw;
	forcount(i,len){
		dat.push(c[i]);
	}
	dat.push(0);
}
string::string(string&& c){
	dat.base= c.dat.base;
	dat.end= c.dat.end;
	c.dat.base= c.dat.end= NULL;
}


string::string(long long v){
	*this= strfmt("%8lli", v);
};
string::string(double v){
	*this= strfmt("%4.4f", v);
};
string::string(rati r){
	*this= strfmt("[%i/%i]",r.num,r.den);
}

string strfmt(char const* fmt, ...){
	string ret;
	ret.dat.alloc(STRFMTMAX);
	va_list vargs;
	va_start(vargs, fmt);
	vsnprintf(ret.dat.base, STRFMTMAX, fmt, vargs);
	return ret;
};



void string::operator=(char const* c){
	dat.clear();
	size_t l= strlen(c);
	forcount(i,l){
		dat.push(c[i]);
	}
	dat.push(0);
}
void string::operator=(string const& c){
	operator=(c.dat.base);
}


string string::operator+(char const* that) const{
	string ret;
	size_t const lthis= size();
	size_t const lthat= strlen(that);
	size_t const lnew= lthis+lthat;
	ret.dat.alloc(lnew+1);
	assert(ret.dat.size()==0);

	forcount(i,lthis)
		ret.dat.push(dat.base[i]);
	forcount(i,lthat)
		ret.dat.push(that[i]);
	ret.dat.push(0);

	return ret;
}
bool string::operator==(string const& that) const{
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

string string::operator+(string const& s) const{
	return operator+(s.dat.base);
}
string& string::operator<<(char const* s){
	*this= *this+s;
	return *this;
}
string& string::operator<<(string const& s){
	return operator<<(s.dat.base);
}




hash_t hash(char const* s){
	//djb2
	unsigned int x= 5381;
	char c;
	while(!!(c=*s++))
		x= (x<<5)+x+c;
	return x;
}
hash_t hash(string const& s){
	return hash(s.dat.base);
}
}