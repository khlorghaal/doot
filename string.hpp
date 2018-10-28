#pragma once
namespace doot{
struct string{
	char* cstr;
	unsigned int len;

	string();
	string(char const*const);
	string(string const&);
	string(long long);
	string(double);
	string(int v): string((long long) v){}
	string(float v): string((double) v){}

	~string();

	string cpy();
	void operator=(char const*);
	void operator=(string const&);

	string operator+(char const*) const;
	string operator+(string const&) const;

	string& operator<<(char const*);
	string& operator<<(string const&);

	inline bool operator!() const { return len==0; };
	bool operator==(string const&) const;
};

string strfmt(char const* fmt, ...);

unsigned int hash(char const*);
inline unsigned int hash(string s){ return hash(s.cstr); }
}