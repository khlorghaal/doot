#pragma once
#include "doot.hpp"
#include "/usr/include/math.h"

#undef max

extc{
	              extern           int   abs (          int __x) noexcept (true) __attribute__ ((__const__)) ;
	              extern      long int  labs (     long int __x) noexcept (true) __attribute__ ((__const__)) ;
	__extension__ extern long long int llabs (long long int __x) noexcept (true) __attribute__ ((__const__));
}

namespace doot{

#define INF INFINITY
cex f64 PI=  M_PI;
cex f64 TAU= M_PI*2;
cex f64 ELR= 2.71828182845904523536;
cex f64 PHI= 1.61803398874989484820;
cst f64 SRT2= sqrt(2.);
cst f64 SRT2RCP= (1./SRT2);

//epsilon equality
#define eps(a,b) (abs(f32(a-b))<ETA)

using rad= f32;
inl rad op""_rad(double   long      x){ re (f32) x; }
inl rad op""_rad(unsigned long long x){ re (f32) x; }
inl rad op""_trn(double   long      x){ re (f32)(x* TAU); }
inl rad op""_trn(unsigned long long x){ re (f32)(x* TAU); }
inl rad op""_deg(double   long      x){ re (f32)(x*(TAU/360)); }
inl rad op""_deg(unsigned long long x){ re (f32)(x*(TAU/360)); }

#define cppify_xf(y)  inl f32 y(f32 x){ re  y##f   (x); }
#define cppify_fx(y)  inl f64 y(f64 x){ re  f##y   (x); }
#define cppify_fxf(y) inl f32 y(f32 x){ re  f##y##f(x); }
#define cppify_lx(y)  inl i32 y(i32 x){ re  l##y   (x); }
#define cppify_llx(y) inl i64 y(i64 x){ re ll##y   (x); }

cppify_xf(sin)
cppify_xf(cos)
cppify_llx(abs) cppify_fx(abs) cppify_fxf(abs)

inl f64 mod(f64 x,f64 m){ re fmod(x,m); }
inl f32 mod(f32 x,f32 m){ re fmodf(x,m); }



tplt T sign(T cst x){
	re (x>0)-(x<0);
	//todo intrinsic
}

tplt T min(T cst a, T cst b){
	bool a_le= a<=b;
	bool b_lt= !a_le;
	re a_le*a + b_lt*b;
}
tplt T max(T cst a, T cst b){
	bool a_ge= a>=b;
	bool b_gt= !a_ge;
	re a_ge*a + b_gt*b;
}

tplt T minabs(T cst a, T cst b){
	bool a_le= abs(a)<=abs(b);
	bool b_lt= !a_le;
	re a_le*a + b_lt*b;
}
tplt T maxabs(T cst a, T cst b){
	bool a_ge= abs(a)>=abs(b);
	bool b_gt= !a_ge;
	re a_ge*a + b_gt*b;
}

//result will have the sign of a
tplt T minabssa(T cst a, T cst b){
	bool a_le= abs(a)<=abs(b);
	bool b_lt= !a_le;
	T sa= sign(a);
	T r= a_le*a + b_lt*b;
	re sa*r;
}
tplt T smaxabssa(T cst a, T cst b){
	bool a_ge= abs(a)>=abs(b);
	bool b_gt= !a_ge;
	re a_ge*a + b_gt*b;
}


tplt T clamp(T cst x, T cst a, T cst b){
	bool lt_a= x<a;
	bool gt_b= x>b;
	bool same= !(lt_a||gt_b);
	re x*same + a*lt_a + b*gt_b;
}

tplt T lerp(float cst x, T cst a, T cst b){
	re a+(b-a)*x;
}

tplt T lerp(   	   T cst c, T cst d,
		f32 cst y, T cst a, T cst b,
				 f32 cst x){
	T cd= c+(d-c)*x;
	T ab= a+(b-a)*x;
	re ab+(cd-ab)*y;
}
#define lerp_field(m) ret.m= lerp(t, a.m, b.m)

tplt T perp(f32 x, T cst a, T cst b, T cst c){
	T ca;
	T cb;
	T cc;
	ass(false);
}
tpl<typn A, typn T> struct lerpolant{
	A a0, a1;
	T t0, t1;
	void bump(A a, T t){
		bump(a0,a1,a);
		bump(t0,t1,t);
	}
	T get(T t){
		T dt= t1-t0;
		t= (t-t0)/dt;
		t= lerp(t, t0,t1);
		re lerp(t, a0,a1);
	}
};

tpl<typn A, typn T> struct perpolant{
	A a0, a1, a2;
	T t0, t1, t2;
	void bump(A a, T t){
		bump(a0,a1,a2,a);
		bump(t0,t1,t2,t);
	}
	A get(T t){
		T dt= t2-t0;
		t= (t-t1)/dt;

		t= perp(t1-t, t0,t1);
		re perp(t, t0,t1);
	}
};

//signed modulus [-m,m)
inl f32 smod(f32 x, f32 m){
	re mod(x+m,m*2)-m;
}
//angle normalize (-pi, pi]
inl rad angn(rad t){
	re t-ceil(t/TAU-.5)*TAU;
}

//( -b +- sqrt(b^2-4ac) )/ 2a
inl f64 quadratic(f64 a, f64 b, f64 c){
	f64 cst srt_det= sqrt(b*b - 4*a*c);
	f64 cst denom= 1./(2*a);
	f64 A= ( -b - srt_det )*denom;
	f64 B= ( -b + srt_det )*denom;
	if(A!=A)
		A= 0;
	if(B!=B)
		B= 0;
	re max(A,B);
}
inl f64 quadratic_ac(f64 a, f64 c){
	//c will be negative in kinematic situations
	re -sqrt(c/a);
}
inl f64 quadratic_ab(f64 a, f64 b){
	re -b/a;
}

inl f32 invsqrt(){
	//todo intrinsic
	ass(false);
	re 0;
}

extern thread_local hash_t frand_seed;
inl f64 frand(){
	re ((f32)hash(frand_seed++))/HASH_MAX_F;
}
inl f64 sfrand(){
	re frand()*2.f-1.f;
}

inl f32 fract(f32 x){ re x-(i32)x; }
inl f64 fract(f64 x){ re x-(i64)x; }

struct rati{
	i32 num, den;

	op int() cst{ re num/den; }

	rati rcp() cst{ re {den,num};	}
	rati op-() cst{	re {-num,den}; }
	rati op*(rati cre x) cst{ re {num*x.num, den*x.den};	}
	rati op/(rati cre x) cst{ re {num*x.den, den*x.num}; }
	rati op+(rati cre x) cst{
		//a/b + c/d
		//ad/bd + cb/db
		//ad+cb / bd
		cst i32 a= num;
		cst i32 b= den;
		cst i32 c= x.num;
		cst i32 d= x.den;
		re {a*d+b*c, b*d};
	}
	rati op-(rati cre x) cst{
		cst i32 a= num;
		cst i32 b= den;
		cst i32 c= x.num;
		cst i32 d= x.den;
		re {a*d-b*c, b*d};
	}

	rati op+(i32 cre x) cst{ re {num+x*den, den}; }
	rati op-(i32 cre x) cst{ re {num-x*den, den}; }
	rati op*(i32 cre x) cst{ re {x*num,den};	}
	rati op/(i32 cre x) cst{ re {num,den*x};	}
};
inl i32 trunc(rati cre r){ re (int)r; }

struct fixed{
	i64 n;
	static cex i64 ZERO= 0ull;
	static cex i64 ONE= 1ull<<31;
	static cex i64 ALMOST_ONE= ONE-1;
	static cex i64 MINVAL= (1ull<<63)+1;
	static cex i64 MAXVAL= MINVAL-1;

	cex fixed(i64 n_):  n( n_ ){}
	cex fixed(u64 n_): n( n_ ){}
	cex fixed(int i):     n( i ){}
	cex fixed(double d):  n( d*ONE ){}
	cex fixed(rati r):    n( (r.num<<31)/r.den ){}

	fixed op+(i64 x){ re n+(x<<31); }
	fixed op-(i64 x){ re n-(x<<31); }
	fixed op*(i64 x){ re n*x; }
	fixed op/(i64 x){ re n/x; }

	fixed op+(fixed x){ re n+x.n; }
	fixed op-(fixed x){ re n-x.n; }
	//n = g(x,y)*k = f(x*k,y*k)
	fixed op*(fixed x){
		re 
			 n*(x.n>>31)+//upper
		   ((n*(x.n&ALMOST_ONE))>>31);//lower
	}
	fixed op/(fixed x){
		re 
			 n/(x.n>>31)+
		   ((n/(x.n&ALMOST_ONE))>>31);
	}
};
inl i32 trunc(fixed x){
	re x.n>>31;
}
inl i32 fract(fixed x){
	re x.n&fixed::ALMOST_ONE;
}

struct cmplx{
	f32 r,i;
	inl cmplx op+(cmplx& z) cst{
		re {r+z.r, i+z.i};
	}
	inl cmplx op*(cmplx& z) cst{
		cst float ar= r, ai= i, br=z.r, bi=z.i;
		re {
			ar*br - ai*bi,
			ar*bi + ai*br
		};
	}
};

}