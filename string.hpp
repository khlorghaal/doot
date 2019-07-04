#pragma once
#include "vector.hpp"

namespace doot{
struct rati;

//null terminated for c friendness
struct string{
	vector<char> dat;

	string();
	string(char const*const);
	string(string&)= delete;
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

	string operator+(char const*) const;
	string operator+(string const&) const;

	string& operator<<(char const*);
	string& operator<<(string const&);

	bool operator!() const { return dat.base==NULL || dat.size()==0; };
	bool operator==(string const&) const;

	operator char*(){ return dat.base; }

	size_t size() const{ return dat.size()-1; }//because null terminator
};

string strfmt(char const* fmt, ...);



unsigned int hash(char const*);
inline unsigned int hash(string s){ return hash(s.dat.base); }



template<typename T>
vector<T>::operator string(){
	string ret;
	ret<<"[ "
		for(auto const& i: *this)
			ret<<*i<<", ";
	ret<<" ]";
}
template<>
inline vector<char>::operator string(){
	string ret;
	bool hadnull= base[size()-1]==0;

	for(auto& e : *this)
		ret.dat.push(e);
	if(!hadnull)
		ret.dat.push(0);

	return ret;
}
}