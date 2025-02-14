#pragma once
#include "defs.hpp"
#include "primitives.hpp"
#include "pp.h"

//extern int dootmain(list<str>& args); //commented because list<str> dep

#ifndef NDEBUG
	#define DOOT_DEBUG 1
	#ifndef DEBUG
		#define DEBUG 1
#endif

#define print(x) cout((str)x)

#define   likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

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
#define unreachable err("unreachable");

inl void break_(){};//for shite ide

inline void nop(){}//for setting breakpoints


#ifdef DEBUG
	#define assert(X) if(!unlikely(X)){err("ass failure");}
#else
	#define assert(X) {}
#endif



struct nocopy{
	nocopy()= default;
	nocopy(  nocopy const&)= delete;
	void op=(nocopy const&)= delete;
};
struct nomove{
	nomove()= default;
	nomove(  nomove const&&)= delete;
	void op=(nomove const&&)= delete;
};
struct nonew{
	static void* op new  (unsigned long)= delete;
	static void* op new[](unsigned long)= delete;
};
struct noassign{
	void op=(noassign const&)= delete;
	void op=(noassign &&)=     delete;
};
struct nodefaultctor{
	nodefaultctor()= delete;
};
struct inplace: nocopy,nomove,nonew,noassign{};
struct noref{
	noref&    op&() = delete;
    noref cre op&() cst = delete;
};

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
void* _malloc( siz  s);
void  _free(   void* p);
void* _realloc(void* p,   siz s);
void  _memcpy( void* dst, void* src, siz len);
void  _memclr( void* dst, siz len);
void  _memset( void* dst, u8 v, sizt len);

//these dont give a fuck about move semantics
tplt void copy(T& dst,T& src){
	_memcpy(&dst,&src,TSIZ);}
tplt void swap(T& a, T& b){
	cex int s= TSIZ;
	u8 c[s];
	_memcpy(&c,&a,s);
	_memcpy(&a,&b,s);
	_memcpy(&b,&c,s);}
inl void bump(are a, are b, are c){
	a=b;b=c;}
inl void bump(are a, are b, are c, are d){
	a=b;b=c;c=d;}

//reinterpret cast
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
		re 1;
	x= __builtin_clzll(x);
	x= 64-x;
	ass(x<63);
	re 1<<x;
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

#define NOTHING
//distinct from void; ie void f(NOTHING NOTHING)

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
but now decent,? not needing comment?

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
OPAQ_MTHD(A,f,  int, float);
OPAQ_MTHD(A,g, void,  void);
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
//previously used Xes for commas
// but that was abhorrent
// use a compound type if needed
#define OPAQ_MTHD(T,M, ARGT,ARG,RETT)\
	RETT T::M(ARGT ARG){\
		ass(!!opaq);\
		re ((_##T*)opaq)->M(ARG);}

//#define OPAQ_T_DEREF(v,T,s)
//	auto _t= ((_##T*)v);\
//	T& s= *((T*)(*((void**)v)));
		/// dont remember what the fuck this is

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
#define let(S) if(S;1)
#define let(S) if(S;1)
#define let(S) if(S;1)
#define let_expr(S) if( ([&]{ (S); re true;})() ) //=> (S);
//range
#define ra( o,n    ) for(i64 o=0   ; o<(n ); o++)
#define ra2(o,n1,n2) for(i64 o=(n1); o<(n2); o++)
//range descending
#define rd(o,n) for(i64 o=(n); o-->0;)
#define each(o,v) for(are o : v)
#define eachd(o,v) \
	for(idx _i##o= v.size(); _i##o-->0;)\
		let(are o= v[_i##o])
#define each2(o,vv) \
	each(vv##o,vv)\
		each(o,vv##o)

//enumerate
#define en(i,o,v) \
	ra(i,v.size())\
		let(are o=v[i])
#define en_d(i,o,v) \
	rd(i,v.size())\
		let(are o=v[i])

#define zip(a,b, la,lb) \
	if( la.size()==lb.size() )\
		ra(_i##a##b, la.size())\
			let(are a=la[_i##a##b])\
				let(are b=lb[_i##a##b])

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

#define voidmap(vec,f,a...) each(_e,vec){ _e.f(a); }

//static condition //generally avoid since should do something else
tpl<bool v> struct scond{ sex bool val= v; };
tplt struct pointyT    : scond<false>{};
tplt struct pointyT<T*>: scond< true>{};

//not elegant, but robust
tplt struct maybe{
	T* t= null;
	//maybe(T   t_)= delete; del ctor causes ambiguity oddly
	maybe(T&  t_):t(&t_){}
	maybe(T&& t_)= delete;//for deteled use _val
	maybe(T*  t_):t( t_){}
	maybe(     ):t(null){}

	bool op!(){ re !t; }
	op bool(){ re !op!();}
	T& op()(T&& none){ re !!t?*t:none; }
	T& op()(T & none){ re !!t?*t:none; }
	//using (void) -> T::none would require SFINAE which i dont fuck with
	T& un(){
		ass(!!t); re *t; }
};
//for primitives that cannot nully intrinsic
tplt struct maybe_val{
	struct val{ T t; };
	val* v;

	maybe_val(maybe_val<T>&& m)
	                 :v(m.v){ m.v=null; }
	//maybe_val(T   t_):v(new val(     t_)){}
	maybe_val(T&  t_):v(new val(     t_)){}
	maybe_val(T&& t_):v(new val((T&&)t_)){}
	maybe_val(      ):v(null){}
	//default ctor too ambiguous

	bool op!(){ re !v; }
	op bool(){ re !!v;}
	T op()(T none){  re !!v?v->t:none; }
	T un(){ass(!!v); re v->t; }
};
tplt struct maybe_i{
	sex T none= nullidx;
	T t=  none;
	        bool op!(){  re t==none; }
	T un(){ ass(!op!()); re t; }
};
tplt using mai= maybe_i<  T>;
tplt using mav= maybe_val<T>;

//unwrappers
//bracketed
#define may_if(s,m)\
	let(ato _may=m)\
	if(likely(!!_may))\
		let(s=  _may.un())
//if null return
#define may_re(s,m)\
	if(unlikely(!m)) re;\
	s= m.un();
//if null return v
#define may_re_v(s,m,v)\
	if(!m) re v;\
	s=  m.un();
//if null return false
#define may_re_f(s,m)\
	if(!m) re false;\
	s=  m.un();
//if null continue
#define may_cont(s,m)\
	if(unlikely(!m)) continue;\
	s=  m.un();
//if null nope
#define may_nope(s,m)\
	if(!m) nope;\
	s=  m.un();
//null coalesce
#define may_nuco(m,e)\
	( (m)? (m).un().e : decltype(m){} )
#define may_conu(m,e,n)\
	( (m)? (m).un().e : (n) )

tplt struct cons{
         T   a;
    cons<T>* b;
};
tplt cons<T>* tail(cons<T>* r){
	for(cons<T>* i= r; i; i=i->b)
		 r= i;
	retr;
}

//nondefault fields and associative ctor for, as init list fwd unviable
//#define CONSTRUCT2(T, T0,F0, T1,F1) \
//	T0 F0; T1 F1; \
//	T(T0 )

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
		x= a0^a1;
	u64 y= a2^a3;
	y^= (y<<16)|(y>>16);
	x^= y;
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
		x= a0^a1;
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
//namespace doot::doot= ::doot;
	//hack, it sometimes gets nested i dont know why, -E didnt show it
	//nested namespace resolution can cause compiler bugs


#ifndef _DOOT_NO_NEW_
	/*placement new, substitute header <new>
	i guess an intercept in addition to other `new` override??
	maybe for heap debug of placements?
	container orientation obviates this*/
	inline void* op new  (unsigned long, void* p) noexcept { return p; }
	inline void* op new[](unsigned long, void* p) noexcept { return p; }
#endif
