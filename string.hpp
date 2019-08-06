#pragma once
#include "vector.hpp"

namespace doot{
struct rati;

//null terminated for c friendness
struct string{
	vector<char> dat;
	char const* cstr() const{ return dat.base; }

	string();
	string(char const*const);
	string(string&);
	string(string&&);
	string(long long);
	string(double);
	string(int v): string((long long) v){}
	string(float v): string((double) v){}
	string(rati);

	~string();

	string cpy();
	void operator=(char const*);
	void operator=(string const&);
	void operator=(vector<char> const&);

	string operator+(char const*) const;
	string operator+(string const&) const;

	string& operator<<(char const*);
	string& operator<<(string const&);

	bool operator!() const { return dat.base==NULL || dat.size()==0; };
	bool operator==(string const&) const;

	operator char const*(){ return dat.base; }

	size_t size() const{ return dat.size()-1; }//because null terminator
};

string strfmt(char const* fmt, ...);



unsigned int hash(char const*);
inline unsigned int hash(string s){ return hash(s.dat.base); }




struct endl_t{};
constexpr endl_t endl;
struct charstream{};
extern charstream cnsl;

charstream& operator<<(charstream&,string const&);
inline charstream& operator<<(charstream& c,char const*const x){ return c<<string(x); };
inline charstream& operator<<(charstream& c,double x){ return c<<string((double)x); };
inline charstream& operator<<(charstream& c,float  x){ return c<<string((double)x); };
inline charstream& operator<<(charstream& c, int8  x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, int16 x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, int32 x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, int64 x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c,uint8  x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c,uint16 x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c,uint32 x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c,uint64 x){ return c<<string((long long)x); };
charstream& operator<<(charstream&,endl_t const&);


}