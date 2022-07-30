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

#ifndef DOOT_NOCOMBINATORIAL
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
#endif

int dootmain(int argc, char** argv);

namespace doot{

void run_tests();
void create_console();

//warn the developer of something odd
void warn(char const*);
//warn the user the application errored and may have invalid state
void bad();
void bad(char const*);
//crash this program with no survivors
void err();
void err(char const*);


inline void nop(){}//for setting breakpoints

inline void ass(bool x) {
#ifdef DEBUG
	if(!x)
		err("failed assert");
#else
	;
#endif
}


#define TSIZ sizeof(T)
#define lengthof(T) (sizeof(T)/sizeof(T[0]))

#define retthis return *this

#define op operator
#define tpl  template
#define typn typename
#define tplt template<typename T>
#define tple template<typename E>

struct no_copy{
	no_copy()= default;
	no_copy(no_copy const&)= delete;
	void operator=(no_copy const&)= delete;
};
struct no_move{
	no_move()= default;
	no_move(no_move const&&)= delete;
	void operator=(no_move const&&)= delete;
};
struct no_new{
	static void* operator new  (sizt)= delete;
	static void* operator new[](sizt)= delete;
};
struct no_assign{
	void operator=(no_assign const&)= delete;
	void operator=(no_assign &&)=     delete;
};
struct no_default_ctor{
	no_default_ctor()= delete;
};

/*containers
may never have their heap owned by two objects
 owner being the object which destructs the heap upon its destruction
element type must be movable, otherwise placement new and thus STL is required
container itself must be movable, for constructing nested containers

containers are the only constructs which may preform pointer operations other than
deref and null check
*/
struct container{
	container()= default;
	container(container const& m)= delete;//copy ctor
	container(container&& m)= default;//move ctor
	//fucking hell i dont want to fuck with constructor polymorph anymore
	container& operator=(container const&)= delete;//copy assign
	container& operator=(container&&)= delete;
	//container& operator=(container&&)= default;//move assign //only direct allocators overload this
};

//on raw memory, bypassing any construction or destruction
//should only be used by containers
//dont use unless you know what youre doing
extern void _memcpy(void* dst,void* src,sizt len);
template<typename T>
inline void copy(T& dst,T& src){
	_memcpy(&dst,&src,TSIZ);
}

template<typename T>
void swap(T& a, T& b){//args must be constructed
	constexpr int s= TSIZ;
	ui8 c[s];
	_memcpy(&c,&a,s);
	_memcpy(&a,&b,s);
	_memcpy(&b,&c,s);
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


ui64 unendian(ui64);
ui32 unendian(ui32);
ui16 unendian(ui16);

constexpr ui32 RANDMAX= ui32(-1);
inline ui32 rand(ui32 x){
	x= ((x>>16)^x)*0x7FFFFFFFU;
	x= ((x>>16)^x)*0x7FFFFFFFU;
	x=  (x>>16)^x;
	return x;
}
inline float rand(float in){
	return (float)rand(*(ui32*)&in) / (float)RANDMAX;
}

//next power of 2
inline sizt nxpo2(sizt x){
	x= __builtin_clzll(x);
	x= 64-x;
	ass(x<63);
	return 2<<x;
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
struct eidptr: idptr<T>{};
template<typename T>
struct ecidptr: ecid{
	ecidptr(ecid a){ e=a.e; i=a.i; };
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

#define OPAQUE(T)
void* _;
 T(){};
~T(){};

#define OPAQUE_CDTOR_DEFAULT(T,X) \
T:: T(){ new((X*)_)X(); }\
T::~T(){ ((X*)_)->~##X(); }

#define FPTR(SYM,ARG,RET) RET(*SYM)(ARG)

/*(A->void)->(B->void)
used to opaquify a function pointer
by wrapping T into f(void*)
when f is called it casts void* to T
the opaqueness is bidirectional
	the header neednt know implementation
	the implementation neednt know header 
useful for passing an op to a thread

header
	extern _f(void* a, void*->void f)
	called by
	g<T,f>{ _f( void*, &voidpfunct<A,f> ) }
implementation
	f(a) invoked, presumed async
	where a may be mutated
*/
template<typename T, FPTR(f,T,void)>
void voidpfunct(void* a){ f(*(T*)a); }

#define CALL_T(SYM,ARG,RET) \
struct SYM{\
	FPTR(f,ARG,RET);\
	ARG x;\
	RET invoke(){ return f(x); }\
};

//range
#define RA(o,n) for(i64 o=0; o<(n); o++)
//range descending
#define RD(o,n) for(i64 o=(n)-1; o>-1; o--)
#define EACH(o,v) for(auto& o : v)
//enumerate
#define EN(i,o,v) i64 i=-1; EACH(o,v){ i++;


#define ZIP(a,b, la,lb) \
for(int _i=0; _i!=la.size(); _i++){\
	auto& a= la[_i];\
	auto& b= lb[_i];

#define ZIP3(a,b,c, la,lb,lc) \
for(int _i=0; _i!=la.size(); _i++){ \
	auto& a= la[_i];\
	auto& b= lb[_i];\
	auto& c= lc[_i];

/*functional macros
 DANGER see examples before hurting yourself

i chose macros over templates for functors.
macros allow simple declarative lambdas, while templates quickly become arcane.
macros ultimately expand to strong types, making explicit type redundant.
and frankly i find template syntax arbitrary and incomprehensible.
*/

//puts memeber into scope
#define BIND(o,m) auto& m= (o).m;

//#define FWD_CAST(R,M,A,B) R M(A a){ return M((B)a); }

#define _CURRY2_h(F) F(y)
#define _CURRY2_g(F) F(x)
//CURRY2(F) => F( g(h(x)) )
#define CURRY2(F) _CURRY2_x(F)

//#define __MAP(l)  
//#define __MAP(l,a) l(a)
#define MAP(l,a,...) __MAP(l,a) __MAP(l,__VA_ARGS__)


#define _FUNCTOR_INVOKE_T0_A1(T) EVAL(T< >(x));
#define _FUNCTOR_INVOKE_T1_A0(T) EVAL(T<x>( ));

//MAP_INVOKE_UNARY(f,a,b,c) => f(a); f(b); f(c);
#define MAP_INVOKE_UNARY(T,...)    MAP(_FUNCTOR_INVOKE_T0_A1(T), __VA_ARGS__)
//MAP_INVOKE_TEMPLATE(T,a,b,c) => T<a>(); T<b>(); T<c>();
#define MAP_INVOKE_TEMPLATE(T,...) MAP(_FUNCTOR_INVOKE_T1_A0(T), __VA_ARGS__)

/*broken and confusing
namespace functor{

template<typename LAMBDA, typename ...LIST>
inline void MAP(){ LAMBDA<LIST...>(); }

template<typename T, typename F> inline void INVOKE_NULLARY( ){ T::F( ); }
template<typename T, typename F> inline void INVOKE_UNARY(T t){ T::F(t); }

template<typename ...LIST>
void MAP_INVOKE_NULLARY(){ MAP<INVOKE_NULLARY,LIST>(); }
template<typename typename ...LIST>
void MAP_INVOKE_UNARY(){ MAP<INVOKE_NULLARY,LIST>(); }
*/


}//namespace end


/*placement new, substitute header <new>
i guess an intercept in addition to other `new` override??
maybe for heap debug of placements?
container orientation obviates this*/
inline void* op new  (doot::sizt, void* p){ return p; }
inline void* op new[](doot::sizt, void* p){ return p; }
