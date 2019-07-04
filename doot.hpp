#pragma once
#include "primitives.hpp"

#ifndef DOOT_NOMACRO
#ifndef NDEBUG
	#define DOOT_DEBUG 1
	#ifndef DEBUG
		#define DEBUG 1
	#endif
#endif
#endif

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

void run_tests();
void create_console();

//warn the user the application errored
void bad();
//crash this program with no survivors
void error();
void error(char const*);


inline void nop(){}//for setting breakpoints

void assert(bool);



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

//convenient, opaque, type-safe indirection
template<typename T>
struct idptr{
	id i;
	idptr(): i(NULLID){};
	idptr(id i_): i(i_){};
	operator id(){ return i; }
	void operator=(id i_){ i= i_; }
	T* operator->(){ return &operator*(); };
	bool operator!(){ return i==NULLID; };
	T& operator*();
};
template<typename T>
struct ehidptr: ehid{
	ehidptr(ehid a){ e=a.e; i=a.i; };
	T* operator->(){ return &operator*(); };
	T& operator*();
};

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

#ifndef DOOT_NOMACRO

#define forcount(o,N) for(int o=0; o<N; o++)
//"count" causes terrible base namespace collisions

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
