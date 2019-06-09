#pragma once
#include <stdlib.h>
#include <string.h>
#include "string.hpp"
#include "primitives.hpp"

#ifndef DOOT_NOMACRO
#ifndef NDEBUG
	#define DOOT_DEBUG 1
	#ifndef DEBUG
		#define DEBUG 1
	#endif
#endif
#endif

#define doot doot
#define dooT doot
#define doOt doot
#define doOT doot
#define dOot doot
#define dOoT doot
#define dOOt doot
#define dOOT doot
#define Doot doot
#define DooT doot
#define DoOt doot
#define DoOT doot
#define DOot doot
#define DOoT doot
#define DOOt doot
#define DOOT doot
#define DOOOT doot;

namespace doot{

#ifndef DOOT_NO_COUT
#ifdef _IOSTREAM_
#error
#endif
struct endl_t{};
constexpr endl_t endl;
struct charstream{};

charstream& operator<<(charstream&, string const&);
inline charstream& operator<<(charstream& c, char const*const x){	return c<<string(x); };
inline charstream& operator<<(charstream& c, double x){ return c<<string((double)x); };
inline charstream& operator<<(charstream& c, float  x){ return c<<string((double)x); };
inline charstream& operator<<(charstream& c, int8  x){  return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, int16 x){  return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, int32 x){  return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, int64 x){  return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, uint8  x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, uint16 x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, uint32 x){ return c<<string((long long)x); };
inline charstream& operator<<(charstream& c, uint64 x){ return c<<string((long long)x); };
charstream& operator<<(charstream&, endl_t const&);

void create_console();

extern charstream cnsl;
#endif

inline void nop(){}//for setting breakpoints

#ifdef DEBUG
	#define assert(b) {\
		if(!(b)){\
			error("Failed Assertion");\
			throw;\
		}}
#else
	#define assert(b) ((void)0)
#endif

/*removed because bad feels bad
extern volatile bool __doot_badstate;
#define badstate (!!__dootbadstate)
#ifdef DEBUG
#define badthrow throw;
#endif
//something happened which left the program in an undefined state
//such as writing/reading outside of allocated space
//logs the badness and sets a flag which things like save functions can check
#define bad(x) cnsl<<"WARNING:"<<__FILE__<<":"<<__LINE__ <<": "<<x<<endl;\
	__doot_badstate= true;\
	badcall();\
	badthrow;
*/
//crash this program with no survivors
#define error(x) {\
	cnsl<<"FATAL ERROR:"<<__FILE__<<":"<<__LINE__ <<": "<<x<<endl;\
	throw;\
}
void runTests();


#define SIZT sizeof(T)
#define SIZEOFT SIZT

#define lengthof(T) (sizeof(T)/sizeof(T[0]))

struct no_copy{
	no_copy()= default;
	no_copy(no_copy &&)=     delete;
	no_copy(no_copy const&)= delete;
};
struct no_new{
	static void* operator new  (size_t)= delete;
	static void* operator new[](size_t)= delete;
};
struct no_assign{
	void operator=(no_assign const&)= delete;
	void operator=(no_assign &&)=     delete;
};
struct no_default_ctor{
	no_default_ctor()= delete;
};


//on raw memory, bypassing any construction or destruction
template<typename T>
void swap(T& a, T& b){
	constexpr int s= SIZEOFT;
	ubyte c[s];
	memcpy(&c,&a,s);
	memcpy(&a,&b,s);
	memcpy(&b,&c,s);
}

template<typename T>
inline void bump(T& t0, T& t1, T& t){
	t0= t1;
	t1= t;
}
template<typename T>
inline void bump(T& t0, T& t1, T& t2, T& t){
	//TODO recursive template
	t0= t1;
	t1= t2;
	t2= t;
}
template<typename T>
inline void copy(T& to, T& from){
	memcpy(&to, &from, SIZEOFT);
}

constexpr uint32 RANDMAX= uint32(-1);
inline uint32 rand(uint32 x){
	x= ((x>>16)^x)*0x7FFFFFFFU;
	x= ((x>>16)^x)*0x7FFFFFFFU;
	x=  (x>>16)^x;
	return x;
}
inline float rand(float in){
	return (float)rand(*(uint32*)&in) / (float)RANDMAX;
}


template<typename A, typename B=A>
struct pair{
	A a;
	B b;
};
template<typename A, typename B=A, typename C=B>
struct triad{
	A a;
	B b;
	C c;
};

/*This can and should be done with templates or macros,
but the variadic syntax is so limited and hideous that
ive condered the vcall overhead of a short list negligible*/
template<typename... T>
struct bus{

};

#ifndef DOOT_NOMACRO

#define zip(a,b, la,lb) \
for(int _i=0; _i!=la.size(); _i++){\
	auto& a= la[_i];\
	auto& b= lb[_i];

#define zip3(a,b,c, la,lb,lc) \
for(int _i=0; _i!=la.size(); _i++){ \
	auto& a= la[_i];\
	auto& b= lb[_i];\
	auto& c= lc[_i];


#define map_literal( lambda, ... ) ()//TODO for item in variad lambda(item)

#endif

}//namespace end
using namespace doot;