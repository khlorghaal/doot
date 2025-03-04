#pragma once

#include "doot.hpp"
//echo "#include <math.h>" | clang++ -e - | clip
extc{

              extern           int   abs (          int __x) noexcept (true) __attribute__ ((__const__)) ;
              extern      long int  labs (     long int __x) noexcept (true) __attribute__ ((__const__)) ;
__extension__ extern long long int llabs (long long int __x) noexcept (true) __attribute__ ((__const__));
extern float  fabsf(float)  noexcept (true) __attribute__ ((__const__)) ;
extern double fabs (double) noexcept (true) __attribute__ ((__const__)) ;
extern double acos (double __x) __attribute__ ((__nothrow__ ));
extern double asin (double __x) __attribute__ ((__nothrow__ ));
extern double atan (double __x) __attribute__ ((__nothrow__ ));
extern double atan2 (double __y, double __x) __attribute__ ((__nothrow__ ));
extern double cos (double __x) __attribute__ ((__nothrow__ ));
extern double sin (double __x) __attribute__ ((__nothrow__ ));
extern double tan (double __x) __attribute__ ((__nothrow__ ));
extern float acosf (float __x) __attribute__ ((__nothrow__ ));
extern float asinf (float __x) __attribute__ ((__nothrow__ ));
extern float atanf (float __x) __attribute__ ((__nothrow__ ));
extern float atan2f (float __y, float __x) __attribute__ ((__nothrow__ ));
extern float cosf (float __x) __attribute__ ((__nothrow__ ));
extern float sinf (float __x) __attribute__ ((__nothrow__ ));
extern float tanf (float __x) __attribute__ ((__nothrow__ ));
extern double fma (double __x, double __y, double __z) __attribute__ ((__nothrow__ ));
extern float fmaf (float __x, float __y, float __z) __attribute__ ((__nothrow__ ));
extern double fmod (double __x, double __y) __attribute__ ((__nothrow__ ));
extern float fmodf (float __x, float __y) __attribute__ ((__nothrow__ ));
extern double floor (double __x) __attribute__ ((__nothrow__ )) __attribute__ ((__const__));
extern float floorf (float __x) __attribute__ ((__nothrow__ )) __attribute__ ((__const__));
extern double ceil (double __x) __attribute__ ((__nothrow__ )) __attribute__ ((__const__));
extern float ceilf (float __x) __attribute__ ((__nothrow__ )) __attribute__ ((__const__));
extern double sqrt (double __x) __attribute__ ((__nothrow__ ));
extern float sqrtf (float __x) __attribute__ ((__nothrow__ ));
extern float powf (float __x, float __y) __attribute__ ((__nothrow__ ));
extern double pow (double __x, double __y) __attribute__ ((__nothrow__ ));
extern double log (double __x) __attribute__ ((__nothrow__ ));
extern double logf (float __x) __attribute__ ((__nothrow__ ));
extern double log2 (double __x) __attribute__ ((__nothrow__ ));
extern double log2f (float __x) __attribute__ ((__nothrow__ ));
extern int finite (double __value) __attribute__ ((__nothrow__ ));
extern int finitef (float __value) __attribute__ ((__nothrow__ ));
extern int isnan (double __value) __attribute__ ((__nothrow__ ));
extern int isnanf (float __value) __attribute__ ((__nothrow__ ));
}

namespace doot{

#define INF INFINITY
cex f64 PI=    3.14159265358979323846;
cex f64 TAU=   6.28318530717958647692;
cex f64 ELR=   2.71828182845904523536;
cex f64 PHI=   1.61803398874989484820;
cex f64 SRT2=  1.41421356237309504880;
cex f64 SRT2R= 0.70710678118654752440;

using rad= f32;//pi-radians
inl rad op""_rad(double   long      x){ re (f32) x; }
inl rad op""_rad(unsigned long long x){ re (f32) x; }
inl rad op""_trn(double   long      x){ re (f32)(x* TAU); }
inl rad op""_trn(unsigned long long x){ re (f32)(x* TAU); }
inl rad op""_deg(double   long      x){ re (f32)(x*(TAU/360)); }
inl rad op""_deg(unsigned long long x){ re (f32)(x*(TAU/360)); }
inl f32 trn(rad t){ re t/TAU; }
inl f32 deg(rad t){ re t/TAU*360; }



#define cppize_x(y)   inl f64 y(f64 x){ re ::    y   (x); }
#define cppize_xf(y)  inl f32 y(f32 x){ re :: y##f   (x); }//inty|floaty
#define cppize_fx(y)  inl f64 y(f64 x){ re :: f##y   (x); }//floaty
#define cppize_fxf(y) inl f32 y(f32 x){ re :: f##y##f(x); }//inty|floaty

#define cppize_lx(y)  inl i32 y(i32 x){ re :: l##y   (x); }
#define cppize_llx(y) inl i64 y(i64 x){ re ::ll##y   (x); }
#define cppize_x_xf(y) cppize_x(y); cppize_xf(y);
#define cppize_x2_xf2(y) \
	inl f64 y(f64 x,f64 m){ re ::y(   x,m); }\
	inl f32 y(f32 x,f32 m){ re ::y##f(x,m); }

cppize_lx(abs) cppize_llx(abs) cppize_fx(abs) cppize_fxf(abs)
cppize_x_xf(cos) cppize_x_xf(acos)
cppize_x_xf(sin) cppize_x_xf(asin)
cppize_x_xf(tan) cppize_x_xf(atan)
cppize_x_xf(sqrt)
cppize_x_xf(log)
cppize_x_xf(log2)
cppize_x_xf(floor)
cppize_x_xf(ceil)
cppize_x2_xf2(pow)

#define imply1(F,A,B) inl B F(A x){ re F((B)x);}
#define imply1_i_f32(F) \
	imply1(F,i32,f32)\
	imply1(F,i64,f32)\
	imply1(F,u32,f32)\
	imply1(F,u64,f32)

#define imply2(F,A,B,C) inl C F(A x,B y){ re F((C)x,(C)y);}
#define imply2_i_f32(F) \
	///partial combinatorial\
	imply2(F,i32,i32,f32) imply2(F,i32,f64,f32) imply2(F,f64,i32,f32)\
	imply2(F,i64,i64,f32) imply2(F,i64,f64,f32) imply2(F,f64,i64,f32)\
	imply2(F,u32,u32,f32) imply2(F,u32,f64,f32) imply2(F,f64,u32,f32)\
	imply2(F,u64,u64,f32) imply2(F,u64,f64,f32) imply2(F,f64,u64,f32)

imply1_i_f32(cos) imply1_i_f32(acos)
imply1_i_f32(sin) imply1_i_f32(asin)
imply1_i_f32(tan) imply1_i_f32(atan)
imply1_i_f32(sqrt)
imply2_i_f32(mod) imply2_i_f32(smod)


inl f32 fract(f32 x){ re x-(i32)x; }
inl f64 fract(f64 x){ re x-(i64)x; }

//epsilon equality
#define eps(a,b) (abs((a-b))<ETA)

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

inl f32 sat( f32 x){ re clamp<f32>(x, 0,1); }
inl f32 sats(f32 x){ re clamp<f32>(x,-1,1); }

//todo make lerp sig conform to glsl
inl f32 lerp(f32 x, f32 a, f32 b){ re a+(b-a)*x; }
inl f64 lerp(f64 x, f64 a, f64 b){ re a+(b-a)*x; }

#define lerp_ab(m) lerp(t, a.m, b.m)

tplt T bilerp(   	   T cst c, T cst d,
		    f32 cst y, T cst a, T cst b,
		    		 f32 cst x){
	T cd= c+(d-c)*x;
	T ab= a+(b-a)*x;
	re ab+(cd-ab)*y;
}

tplt T perp(f32 x, T cst a, T cst b, T cst c){
	T ca;
	T cb;
	T cc;
	ass(false);//todo
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

inl f64 mod(f64 x,f64 m){ re ::fmod( x,m); }//fmod is the name of the function and not its type, it doesnt fit the schema
inl f32 mod(f32 x,f32 m){ re ::fmodf(x,m); }

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
