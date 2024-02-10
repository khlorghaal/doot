#pragma once
#include "primitives.hpp"

#ifndef DOOT_NOMACRO
#ifndef NDEBUG
	#define DOOT_DEBUG 1
	#ifndef DEBUG
		#define DEBUG 1
	#endif
#endif

#define TSIZ sizeof(T)

#define re return
#define retthis return *this
#define retth   return *this
#define rett    return *this
#define retr   return r
#define retret return ret;

#define inl inline
#define op operator
#define tpl template
#define typn typename
#define tplt template<typename T>
#define tple template<typename E>
#define tples template<typename... E>
#define cst const
#define cre const&
#define acs auto const
#define acr auto const&
#define are auto&
#define cex constexpr
#define sex static constexpr
#define ass assert
#define rcas reinterpret
#define extc extern "C"
#define dtyp decltype

#define FPTR(SYM,ARG,RET) RET(*SYM)(ARG)

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

#ifndef DOOT_NO_MAIN
	extern int dootmain(int argc, char** argv);
#endif

namespace doot{

extern void run_tests();

extern void create_console();

extern cstr strfmt_cstr(cstr,...);

#define _stringize(s) #s
#define stringize(s) _stringize(s)
#define SRCLOC " " __FILE__ ":" stringize(__LINE__) ": "
//spaces required, no spaces invokes operator""
//the nesting acts an an eval, as __LINE__ is an int


//warn the developer of something odd
extern void _warn(cstr,cstr);
#define warn(s) _warn((SRCLOC),s);
//warn the user the application errored and may have invalid state
extern void _bad(cstr,cstr);
#define bad(s) _bad((SRCLOC),s);
//crash this program with no survivors
extern void _err(cstr,cstr);
#define err(s) _err((SRCLOC),s);
#define unreachable err("unreachable")


inline void nop(){}//for setting breakpoints


#ifdef DEBUG
	#define assert(X) {if(!(X)){err("ass failure: " stringize(X));}}
#else
	#define assert(X) {}
#endif



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
	static void* operator new  (unsigned long)= delete;
	static void* operator new[](unsigned long)= delete;
};
struct no_assign{
	void operator=(no_assign const&)= delete;
	void operator=(no_assign &&)=     delete;
};
struct no_default_ctor{
	no_default_ctor()= delete;
};
struct inplace: no_copy,no_move,no_new,no_assign{};

/*containers
may never have their heap owned by two objects
 owner being the object which destructs the heap upon its destruction
move and copy ctors are globally disabled
	i dont want to fuck with that anymore
memcpy is used internally when needed

containers are the only constructs which may preform pointer operations other than
deref and null check
*/
struct container{
	container()= default;
	container(container const& m)= delete;//copy
	container(container&& m)= default;//move
	container& operator=(container const&)= delete;//copy
	container& operator=(container&&)= delete;//move
};


/*raw memory, bypassing any construction or destruction
should only be used by containers
dont use unless know what doing*/

void  _thread(char const* name, void(*f)(void*),void* arg);
void* _malloc( sizt  s);
void  _free(   void* p);
void* _realloc(void* p,   sizt s);
void  _memcpy( void* dst, void* src, sizt len);
void  _memclr( void* dst, sizt len);

//these dont give a fuck about move semantics
tplt inline void copy(T& dst,T& src){
	_memcpy(&dst,&src,TSIZ);
}
tplt void swap(T& a, T& b){
	constexpr int s= TSIZ;
	u8 c[s];
	_memcpy(&c,&a,s);
	_memcpy(&a,&b,s);
	_memcpy(&b,&c,s);
}
tplt inline void bump(T& t0, T& t1, T& t){
	t0= t1;
	t1= t;
}
tplt inline void bump(T& t0, T& t1, T& t2, T& t){
	t0= t1;
	t1= t2;
	t2= t;
}

tpl<typn B, typn A> B reinterpret(A a){
	ass(sizeof(A)==sizeof(B));
	return *((B*)(&a));
}




tplt cex T INTMAX;
tpl<> inl cex  i8 INTMAX< i8> = 0x7f;
tpl<> inl cex i16 INTMAX<i16> = 0x7fff;
tpl<> inl cex i32 INTMAX<i32> = 0x7fffffff;
tpl<> inl cex i64 INTMAX<i64> = 0x7fffffffffffffff;
tpl<> inl cex  u8 INTMAX< u8> = 0xff;
tpl<> inl cex u16 INTMAX<u16> = 0xffff;
tpl<> inl cex u32 INTMAX<u32> = 0xffffffff;
tpl<> inl cex u64 INTMAX<u64> = 0xffffffffffffffff;

tplt cex T HUGE;
tplt cex T TINY;
tpl<> inline cex f32 HUGE<f32> = 1e32f;
tpl<> inline cex f32 TINY<f32> = 1e-32f;
tpl<> inline cex f64 HUGE<f64> = 1e300 ;
tpl<> inline cex f64 TINY<f64> = 1e-300;
cex f32 ETA= 1.e-5;//kinda small but not really


//tplt struct MAP_KIND;
//#define MAPT(S,A,B) tpl<> struct MAP_KIND_##S<A>{ using T= B };
//DEMAPT(S) using MAP_KIND_##S<A>::T
//MAPT(signedT,  u8, i8 )
//MAPT(signedT, u16, i16)
//MAPT(signedT, u32, i32)
//MAPT(signedT, u64, i64)
//MAPT(signedT,   T, T  )


inl hash_t hash(u32 x){
	//murmur
	x*= 0xcc9e2d51;
    x^=(x<<15)|(x>>17);
    x*= 0x1b873593;
	re x;
}
inl hash_t hash(u64 x){
	x= x^(x*0xbf58476d1ce4e5b9ull);
	re hash((u32)x);
}
inl hash_t hash(void* x){ re hash((u64) x); };
inl hash_t hash(i32   x){ re hash((u32) x); };
inl hash_t hash(i64   x){ re hash((u64) x); };

inl u32 rand(u32 x){
	return hash(x);
}
inl float rand(float in){
	return (float)rand(rcas<u32>(in)) / (float)INTMAX<u32>;
}

//next power of 2
inl sizt nxpo2(sizt x){
	x= __builtin_clzll(x);
	x= 64-x;
	ass(x<63);
	return 2<<x;
}

tpl<typn A, typn B=A>
struct pair{
	A a;
	B b;
};
tpl<typn A, typn B=A, typn C=B>
struct triad{
	A a;
	B b;
	C c;
};

/*these are to
-declaration doesnt need implementation header
-implementation doesnt need declaration header

i had to channel soul of saint terry to assist with
was possessed into rubber duck as standard
he called me slurs at least 20 times until i figured out
weild unholy power of void pointer
blessed by his divine intellect
subjugate and contain unholy
may thine codebase be minimized of voodoo

use

//header
struct A{
	OPAQUE_DECL(A) => void* _;
	void f();
}

//impl lib ; within doot namespace
#import "doot..."
OPAQEXTRN_CDTOR(A)
OPAQEXTRN(A,f)

//impl ext ; within std|stl|windows namespace
#import <std...>
OPAQIMPL_CDTOR(A)
OPAQIMPL_F(A,f)

output

//header
struct A{
	void* _;
	 A();
	~A();
	void f();
}
//impl lib
extern void A_CTOR(void*);
extern void A_DTOR(void*);
A:: A(){ A_CTOR(_); }
A::~A(){ A_DTOR(_); }
extern void A_f(void*);
void A::f(){ A_f(_); }

//impl ext
#include <thingA>
using _A= thingA
or
struct _A{ thingA a; void f(){ a.g(); }; }

void A_CTOR(void* p){ new((_A*)p)_A(); };//placement
void A_DTOR(void* p){ ((_A*)p)->~_A(); };
void A_f(void* p){ ((_A*)p)->f(); };
*/
#define OPAQ_CDTOR_DECL(T) T(); ~T();

#define OPAQ_CDTOR_DEFR(T) \
extern void     T##_##CTOR(void*);\
extern void     T##_##DTOR(void*);\
T:: T(){ T##_##CTOR(this);  };\
T::~T(){ T##_##DTOR(this);  };

#define OPAQ_M(M) void* M;
//opaque member to be refereed to
// and casted by implementation
//must be first member of struct
// void* badness requires this

//self void*, member name, typn
#define OPAQ_M_CTOR(v,T) \
*((void**)v)= new T();
#define OPAQ_M_DTOR(v,T) \
delete ( (T*)(*( (void**)v )) );
//the void * is a struct with a void*
//new the T and pass its location
//into the member values
#define OPAQ_T_DEREF(v,T,s) \
T& s= *((T*)(*((void**)v)));
//some of the most horrendous code ive ever written
//this WILL be purged

#define OPAQEXTRN_M(T,F) \
extern void T##_##F(void*); \
inline void T::F(){ T##_##F(_);}
//T_F  the opaque function
//T::F the member function callforwarding to the opaque
#define OPAQIMPL_F(T,F) \
void T##_##F(void* p){ ((T*)p)->F(); }

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

//used for loopy macros to prevent parents mismatch
//horribly renegade
#define fauto(S) if(auto& S;1)

//range
#define RA(o,n) for(i64 o=0; o<(n); o++)
#define RA2(o,n1,n2) for(i64 o=n1; o<(n2); o++)
//range descending
#define RD(o,n) for(i64 o=(n); o-->0;)
#define EACH(o,v) for(auto& o : v)
#define EACHD(o,v) \
	for(idx _i##o= v.size(); _i##o-->0;)\
		fauto(o= v[_i##o])
//um okay so uh yea
// the secondary for is a way of creating a statement
// without the need for a bracket mismatch

//enumerate
#define EN(i,o,v) \
	for(idx i=0; i<v.size(); i++)\
		fauto(o=v[i])
#define EN_D(i,o,v) \
	for(idx i=v.size()-1; i>=0; i--)\
		fauto(o=v[i])

#define ZIP(a,b, la,lb) \
	RA(_i##a##b, la.size())\
		fauto(a=la[_i##a##b])\
			fauto(b=lb[_i##a##b])

//symcats must use the symbol names not the vectors, as the vector symbols may be expressions

//#define ZIP3(a,b,c, la,lb,lc) \
//for(int _i=0; _i!=la.size(); _i++){ \
//	auto& a= la[_i];\
//	auto& b= lb[_i];\
//	auto& c= lc[_i];



#define OPADDSUB \
	tpl<typn... E> void op+=(E cst&... e){ add(e...); }\
	tpl<typn... E> void op-=(E cst&... e){ sub(e...); }


/*functional macros
 DANGER see examples before hurting yourself

i chose macros over templates for functors.
macros allow simple declarative lambdas, while templates quickly become arcane.
macros ultimately expand to strong types, making explicit typn redundant.
but mostly i find template syntax arbitrary and incomprehensible.
*/

//puts memeber into scope
#define usem(o,m) auto& m= (o).m;

//#define FWD_CAST(R,M,A,B) R M(A a){ return M((B)a); }

//TODO test? did i what these?? uh????
#define _CURRY2_h(F) F(y)
#define _CURRY2_g(F) F(x)
//CURRY2(F) => F( g(h(x)) )
#define CURRY2(F) _CURRY2_x(F)

#define VOIDMAP(vec,f,a...) EACH(_e,vec){ _e.f(a); }


/*a primitive that enforces casting
ie seconds and miliseconds, degrees and radians
T: the underlying primitive
C: crtp leaf class
BASIS: the subtype within the scalar-kind with unit = 1
*/
tpl<typn T, typn S> struct scalar{
	T v=0;
	op T(){ re v; }
};
#define SCALAR_RATIO(A,B) *B::unit/A::unit

#define _FUNCTOR_INVOKE_T0_A1(T) EVAL(T< >(x));
#define _FUNCTOR_INVOKE_T1_A0(T) EVAL(T<x>( ));

//MAP_INVOKE_UNARY(f,a,b,c) => f(a); f(b); f(c);
#define MAP_INVOKE_UNARY(T,...)    MAP(_FUNCTOR_INVOKE_T0_A1(T), __VA_ARGS__)
//MAP_INVOKE_TEMPLATE(T,a,b,c) => T<a>(); T<b>(); T<c>();
#define MAP_INVOKE_TEMPLATE(T,...) MAP(_FUNCTOR_INVOKE_T1_A0(T), __VA_ARGS__)

tpl<typn T> struct maybe{
	T* t;
	static maybe<T> yes(T* t){ ass(t!=null); return {   t}; };
	static maybe<T>  no(){                   return {null}; };
	bool op!(){ return !t; }
	T& op()(T& none){ return !!t?*t:none; }
};


//intrinsics

#define   likely(x) __builtin_expect(x, 1)
#define unlikely(x) __builtin_expect(x, 0)


#ifdef _MSVC_LANG
	inline u64 unendian(u64 i){ return _i8swap_u64(i); };
	inline u32 unendian(u32 i){ return _i8swap_ulong(i);  };
	inline u16 unendian(u16 i){ return _i8swap_ushort(i); };
#else
	inline u64 unendian(u64 i){ return __builtin_bswap64(i); };
	inline u32 unendian(u32 i){ return __builtin_bswap32(i); };
	inline u16 unendian(u16 i){ return __builtin_bswap16(i); };	
#endif

}//namespace end


#ifndef _DOOT_NO_NEW_
	/*placement new, substitute header <new>
	i guess an intercept in addition to other `new` override??
	maybe for heap debug of placements?
	container orientation obviates this*/
	inline void* op new  (unsigned long, void* p) noexcept { return p; }
	inline void* op new[](unsigned long, void* p) noexcept { return p; }
#endif
