#pragma once
#include "primitives.hpp"
#include "pp.h"

#ifndef NDEBUG
	#define DOOT_DEBUG 1
	#ifndef DEBUG
		#define DEBUG 1
#endif

#define TSIZ sizeof(T)

#define re return
#define rethis return *this
#define reth   return *this
#define retr   return r
#define retret return ret;

#define inl inline
#define op operator
#define tpl template
#define typn typename
#define tplt  template<typename T>
#define tple  template<typename E>
#define tples template<typename... E>
#define kind tpl<typn> typn
#define cst const
#define cre const&
#define acs auto const
#define acr auto const&
#define are auto&
#define cex constexpr
#define sex static constexpr
#define ass assert
#define ext  extern
#define extc extern "C"
#define dtyp decltype
#define lis list
#define may maybe
#define nope return {}
#define print(x) cout((str)x)

#define   likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

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


#ifdef _MSVC_LANG
	inline u64 unendian(u64 i){ return _i8swap_u64(i); };
	inline u32 unendian(u32 i){ return _i8swap_ulong(i);  };
	inline u16 unendian(u16 i){ return _i8swap_ushort(i); };
#else
	inline u64 unendian(u64 i){ return __builtin_bswap64(i); };
	inline u32 unendian(u32 i){ return __builtin_bswap32(i); };
	inline u16 unendian(u16 i){ return __builtin_bswap16(i); };	
#endif

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
//panic. crash this program with no survivors.
extern void _err(cstr,cstr);
#define err(s) _err((SRCLOC),s);
#define unreachable err("unreachable")


inline void nop(){}//for setting breakpoints


#ifdef DEBUG
	#define assert(X) if(!unlikely(X)){err("ass failure");}
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
	_memcpy(&dst,&src,TSIZ);}
tplt void swap(T& a, T& b){
	constexpr int s= TSIZ;
	u8 c[s];
	_memcpy(&c,&a,s);
	_memcpy(&a,&b,s);
	_memcpy(&b,&c,s);}
tplt inline void bump(T& t0, T& t1, T& t){
	t0= t1;
	t1= t;}
tplt inline void bump(T& t0, T& t1, T& t2, T& t){
	t0= t1;
	t1= t2;
	t2= t;}

//reference cast
tpl<typn B,typn A> B& rcas(A& a){
	ass(sizeof(A)==sizeof(B));
	re *((B*)(&a));}
//voidpointer to reference
tplt T& vcas(void* p){
	ass(!!p);
	re *(T*)p;}


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
tpl<> inl cex f32 HUGE<f32> = 1e32f;
tpl<> inl cex f32 TINY<f32> = 1e-32f;
tpl<> inl cex f64 HUGE<f64> = 1e300 ;
tpl<> inl cex f64 TINY<f64> = 1e-300;
cex f32 ETA= 1.e-5;//kinda small but not really


//tplt struct MAP_KIND;
//#define MAPT(S,A,B) tpl<> struct MAP_KIND_##S<A>{ using T= B };
//DEMAPT(S) using MAP_KIND_##S<A>::T
//MAPT(signedT,  u8, i8 )
//MAPT(signedT, u16, i16)
//MAPT(signedT, u32, i32)
//MAPT(signedT, u64, i64)
//MAPT(signedT,   T, T  )


//next power of 2
inl sizt nxpo2(sizt x){
	if(x==0)//UB
		return 1;
	x= __builtin_clzll(x);
	x= 64-x;
	ass(x<63);
	return 1<<x;
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

#define L2S(A,B) A   B
#define L2C(A,B) A , B
#define L2M(A,B) A ; B ;
//lists for macro parameters
//ie passing a list parameter to avoid variadics

#define FPTR(ARG,RET)         RET (*   )(ARG)
#define FPTR_VAR(SYM,ARG,RET) RET (*SYM)(ARG)

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

was some of most horrendous code ive ever written
but now decent, not needing comment

#.hpp
struct A{
	OPAQ_OBJ(A);
	void f();
	int g(float);
}
OPAQ_XTRN_OBJ(A);

#.cpp
struct _A{
	void f();
	int g(float);
}
OPAQ_CDTR(A);
OPAQ_MTHD(A,f);
#define OPAQ_RETT int
#define OPAQ_ARGS float f
#define OPAQ_ARGN       f
OPAQ_MTHD(A,g);
#define OPAQ_RETT void
#define OPAQ_ARGS void
#define OPAQ_ARGN
*/
#define OPAQ_OBJ(T) \
	void* opaq=nullptr; T(); ~T();
#define OPAQ_XTRN_OBJ(T) \
    ext void     T##_##CTOR(void**);\
    ext void     T##_##DTOR(void* );\
	inl T:: T(){ T##_##CTOR(&opaq);};\
	inl T::~T(){ T##_##DTOR( opaq);};
#define OPAQ_CDTR(T) \
	void T##_##CTOR(void** v){\
		ass(!*v);\
		*v= (void*)( new _##T() ); }\
	void T##_##DTOR(void* v){ \
		ass(!!v);\
		delete ((_##T*)v); }
#define OPAQ_ARGS void//must X for commas
#define OPAQ_RETT void
#define OPAQ_ARGN
#define OPAQ_MTHD(T,M)\
	OPAQ_RETT T::M(OPAQ_ARGS){ \
		ass(!!opaq);\
		re ((_##T*)opaq)->M( OPAQ_ARGN);}

//#define OPAQ_T_DEREF(v,T,s)
//	auto _t= ((_##T*)v);\
//	T& s= *((T*)(*((void**)v)));

//T_F  the opaque function
//T::F the member function forwarding to the opaque
#define OPAQIMPL_F(T,F) \
	void T##_##F(void* p){ ((T*)p)->F(); }

#define OPAQ_FPTR_VAR(SYM) FPTR_VAR(SYM,void*,void)
/*(A->void)->(B->void)
used to opaquify a function pointer
by wrapping T into f(void*)
when f is called it casts void* to T
the opaqueness is bidirectional
	the header neednt know implementation
	the implementation neednt know header 
useful for passing an op to a thread

taking the address of the templated function
does not call the function, in most sane uses
it will be interepred as returning a fptr

header
	extern _f(void* a, void*->void f)
	called by
	g<T,f>{ _f( void*, &voidpfn<A,f> ) }
implementation
	f(a) invoked, presumed async
	where a may be mutated

^ ive rewritten a lot and dont know if this is still accurate
*/
#define SVOID //symbolic void
#define CALL_T(S,A,B) \
struct S{\
	using F = FPTR(A,B);\
	F f;\
	A x;\
	B invoke(){ return f(x); }\
};
CALL_T( call_opaq_t, void*, void )
//the void* does returnness; mut void*->void
#define CALL_PACK(S,binds) void _CALL_##S()

#define uses() tplt struct{ using type=T; }

//used for loopy macros to prevent parents mismatch
//horribly renegade //ive been told this isnt abnormal; that is horrifying
#define ifauto(S) if(auto& S;1)
#define ifexpr(S) if( ([&]{ (S); re true;})() ) //=> (S);

//range
#define RA(o,n) for(i64 o=0; o<(n); o++)
#define RA2(o,n1,n2) for(i64 o=n1; o<(n2); o++)
//range descending
#define RD(o,n) for(i64 o=(n); o-->0;)
#define EACH(o,v) for(auto& o : v)
#define EACHD(o,v) \
	for(idx _i##o= v.size(); _i##o-->0;)\
		ifauto(o= v[_i##o])
#define EACH2(o,vv) \
	EACH(vv##o,vv)\
		EACH(o,vv##o)

//enumerate
#define EN(i,o,v) \
	for(idx i=0; i<v.size(); i++)\
		ifauto(o=v[i])
#define EN_D(i,o,v) \
	for(idx i=v.size()-1; i>=0; i--)\
		ifauto(o=v[i])

#define ZIP(a,b, la,lb) \
	if( la.size()==lb.size() )\
		RA(_i##a##b, la.size())\
			ifauto(a=la[_i##a##b])\
				ifauto(b=lb[_i##a##b])

//symcats must use the symbol names not the lists, as the list symbols may be expressions

//#define ZIP3(a,b,c, la,lb,lc) \
//for(int _i=0; _i!=la.size(); _i++){ \
//	auto& a= la[_i];\
//	auto& b= lb[_i];\
//	auto& c= lc[_i];


#define OPADDSUB \
	tpl<typn... E> void op+=(E cre... e){ add(e...); }\
	tpl<typn... E> void op-=(E cre... e){ sub(e...); }

#define LVAL  
#define RVAL const&
#define XVAL &&
#define VTYP_LRX L(LVAL) L(RVAL) L(XVAL)
#define VTYP_RX  L(RVAL) L(XVAL)

/*functional macros
 DANGER see examples before hurting yourself

i chose macros over templates for functors.
macros allow simple declarative lambdas, while templates quickly become arcane.
macros ultimately expand to strong types, making explicit typn redundant.
but mostly i find template syntax arbitrary and incomprehensible.
*/

//puts memeber into scope
#define usem(o,m) are m= (o).m;

#define voidmap(vec,f,a...) EACH(_e,vec){ _e.f(a); }

//not elegant, but robust
tplt struct maybe{
	T* t= null;
	maybe(T& t_):t( &t_){}
	maybe(     ):t(null){}

	bool op!(){ re !t; }
	op bool(){ re !op!();}
	T& op()(T&& none){ re !!t?*t:none; }
	T& op()(T & none){ re !!t?*t:none; }
	//using (void) -> T::none would require SFINAE which i dont fuck with
	T& un(){ ass(!!t); re *t; }
};
//unwrappers
//bracketed
#define may_if(s,m)\
	if(likely(!!m))\
		ifauto(s= m.un())
//if null => {}; discouraged, use ({}) directly
#define may_emp(s,m)\
	are s= m({});
//if null return
#define may_re(s,m)\
	if(unlikely(!m)) re;\
	are s= m.un();
//if null return v
#define may_re_v(s,m,v)\
	if(unlikely(!m)) re v;\
	are s= m.un();
//if null return false
#define may_re_f(s,m)\
	if(unlikely(!m)) re false;\
	are s= m.un();
//if null continue
#define may_cont(s,m)\
	if(unlikely(!m)) continue;\
	are s= m.un();
//if null nope
#define may_nope(s,m)\
	if(!m) nope;\
	are s= m.un();



//essential math

inl hash_t hash(u32 x){
	u32 PRIME_0 = 0x9E3779B1u;
	u32 PRIME_1 = 0x85EBCA77u;
	u32 PRIME_2 = 0xC2B2AE3Du;
	u32 PRIME_3 = 0x27D4EB2Fu;

	u32 a0= x*PRIME_0;
	u32 a1= x*PRIME_1;
	u32 a2= x*PRIME_2;
	u32 a3= x*PRIME_3;
	x^= a0^a1^a2^a3;

	re x;
}
inl hash_t hash(u64 x){
	u64 PRIME_0 = 0xC43A65E071D385B1ull;
	u64 PRIME_1 = 0x9A232294BAEEBB27ull;
	u64 PRIME_2 = 0xAC0671A220DFABF7ull;
	u64 PRIME_3 = 0xB59BC8E3F34FFF6Bull;

	u64 a0= x*PRIME_0;
	u64 a1= x*PRIME_1;
	u64 a2= x*PRIME_2;
	u64 a3= x*PRIME_3;
		x= a0;
	u64 y= a2^a3;
	y^= (y<<32)|(y>>32);
	x^= y;

	re x;
}
inl hash_t hash(void* x){ re hash((u64) x); };
inl hash_t hash(i32   x){ re hash((u32) x); };
inl hash_t hash(i64   x){ re hash((u64) x); };

inl u32 rand(u32 x){
	re hash(x);
}
inl float rand(float in){
	re (float)rand(rcas<u32>(in)) / (float)INTMAX<u32>;
}




}//namespace end
//namespace doot::doot= ::doot;//hack, it sometimes gets nested i dont know why, -E didnt show it


#ifndef _DOOT_NO_NEW_
	/*placement new, substitute header <new>
	i guess an intercept in addition to other `new` override??
	maybe for heap debug of placements?
	container orientation obviates this*/
	inline void* op new  (unsigned long, void* p) noexcept { return p; }
	inline void* op new[](unsigned long, void* p) noexcept { return p; }
#endif
