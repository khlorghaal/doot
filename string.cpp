#include "string.hpp"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include "math.hpp"
namespace doot{

constexpr int STRFMTMAX= 1024;

void strinit(string& s, int len_){
	s.cstr= (char*)malloc(len_+1);
	s.cstr[len_]= 0;
	s.len= len_;
}
string::string(){ strinit(*this, 0); }
string::string(char const*const c){
	len= strlen(c);
	if(len==-1)
		throw;
	cstr= (char*)malloc(len+1);
	memcpy(cstr, c, len+1);
}
string::string(string const& c): string(c.cstr){}


string::string(long long v){
	strinit(*this, STRFMTMAX);
	snprintf(cstr, STRFMTMAX, "%llX", v);
};
string::string(double v){
	strinit(*this, STRFMTMAX);
	snprintf(cstr, STRFMTMAX, "%.4f", v);

};
string strfmt(char const* fmt, ...){
	string ret;
	strinit(ret, STRFMTMAX);
	va_list vargs;
	va_start(vargs, fmt);
	vsnprintf(ret.cstr, STRFMTMAX, fmt, vargs);
	return ret;
};

string::~string(){
	free(cstr);
}


void string::operator=(char const* c){
	if(strlen(cstr)==len)
		free(cstr);
	len= strlen(c);
	cstr= (char*)malloc(len+1);
	memcpy(cstr, c, len+1);
}
void string::operator=(string const& c){
	operator=(c.cstr);
}


string string::operator+(char const* that) const{
	string ret;
	size_t lthis= strlen(cstr);
	size_t lthat= strlen(that);
	ret.len= lthis + lthat;
	ret.cstr= (char*)malloc(ret.len+1);

	memcpy(ret.cstr,       cstr, lthis);
	memcpy(ret.cstr+lthis, that, lthat+1);

	return ret;
}
string string::operator+(string const& s) const{
	return operator+(s.cstr);
}


string& string::operator<<(char const* s){
	*this= *this+s;
	return *this;
}
string& string::operator<<(string const& s){
	return operator<<(s.cstr);
}

bool string::operator==(string const& that) const{
	if(cstr==that.cstr)
		return true;
	if(len!=that.len)
		return false;

	char const* ia=      cstr;
	char const* ib= that.cstr;
	while(*ia){
		if(*ia!=*ib)
			return false;
		ia++;
		ib++;
	}
	return true;
}

unsigned int hash(char const* s){
	//djb2
	unsigned int x= 5381;
	char c;
	while(c=*s++)
		x= (x<<5)+x+c;
	return x;
}
}