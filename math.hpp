#pragma once
#include "doot.hpp"
#include "hash.hpp"
#include "math.h"

//windows.h hacks
#undef min
#undef max
#undef NOMINMAX
#define NOMINMAX 1

namespace doot{
using ::sqrt;
using ::tan;
using ::atan2;
using ::exp;
using ::pow;
using ::log;
using ::log2;
using ::floor;
using ::ceil;

#define INF INFINITY
constexpr double PI=  3.14159265358979323846;
constexpr double TAU= 6.28318530717958647692;
constexpr double ELR= 2.71828182845904523536;
constexpr double PHI= 1.61803398874989484820;
constexpr double BIG= 1ll<<31;
constexpr double SMALL= 1./BIG;
const double SRT2= sqrt(2.);
const double SRT2INV= (1./SRT2);

#define cppify_yf(y) \
inline float y(float x){ return y##f(x); }
#define cppify_fy(y) \
inline double y(double x){ return f##y(x); }
#define cppify_fyf(y) \
inline float y(float x){ return f##y##f(x); }
#define cppify_lly(y) \
inline long long y(long long x){ return ll##y(x); }

cppify_yf(sin)
cppify_yf(cos)
cppify_lly(abs) cppify_fy(abs)
inline double mod(double x,double m){ return fmod(x,m); }
inline float mod(float x,float m){ return fmodf(x,m); }



template<typename T>
inline T sign(T const x){
	return (x>0)-(x<0);
}

template<typename T>
inline T min(T const a, T const b){
	bool a_le= a<=b;
	bool b_lt= !a_le;
	return a_le*a + b_lt*b;
}
template<typename T>
inline T max(T const a, T const b){
	bool a_ge= a>=b;
	bool b_gt= !a_ge;
	return a_ge*a + b_gt*b;
}

template<typename T>
inline T minabs(T const a, T const b){
	bool a_le= abs(a)<=abs(b);
	bool b_lt= !a_le;
	return a_le*a + b_lt*b;
}
template<typename T>
inline T maxabs(T const a, T const b){
	bool a_ge= abs(a)>=abs(b);
	bool b_gt= !a_ge;
	return a_ge*a + b_gt*b;
}

//result will have the sign of a
template<typename T>
inline T minabssa(T const a, T const b){
	bool a_le= abs(a)<=abs(b);
	bool b_lt= !a_le;
	T sa= sign(a);
	T r= a_le*a + b_lt*b;
	return sa*r;
}
template<typename T>
inline T smaxabssa(T const a, T const b){
	bool a_ge= abs(a)>=abs(b);
	bool b_gt= !a_ge;
	return a_ge*a + b_gt*b;
}


template<typename T>
inline T clamp(T const x, T const a, T const b){
	bool lt_a= x<a;
	bool gt_b= x>b;
	bool same= !(lt_a||gt_b);
	return x*same + a*lt_a + b*gt_b;
}

template<typename T>
inline T lerp(float const x, T const a, T const b){
	return a+(b-a)*x;
}

template<typename T>
inline T lerp(   	   T const c, T const d,
		float const y, T const a, T const b,
				   float const x){
	T cd= c+(d-c)*x;
	T ab= a+(b-a)*x;
	return ab+(cd-ab)*y;
}
#define lerp_field(m) ret.m= lerp(t, a.m, b.m)

template<typename T>
inline T perp(float const x, T const a, T const b, T const c){
	T ca;
	T cb;
	T cc;
	ass(false);
}
template<typename A, typename T> struct lerpolant{
	A a0, a1;
	T t0, t1;
	void bump(A a, T t){
		bump(a0,a1,a);
		bump(t0,t1,t);
	}
	T get(time_t t){
		time_t dt= t1-t0;
		t= (t-t0)/dt;
		t= lerp(t, t0,t1);
		return lerp(t, a0,a1);
	}
};

template<typename A, typename T> struct perpolant{
	A a0, a1, a2;
	T t0, t1, t2;
	void bump(A a, T t){
		bump(a0,a1,a2,a);
		bump(t0,t1,t2,t);
	}
	A get(time_t t){
		time_t dt= t2-t0;
		t= (t-t1)/dt;

		t= perp(t1-t, t0,t1);
		return perp(t, t0,t1);
	}
};

//signed modulus [-m,m)
inline double smod(double x, double m){
	return fmod(x+m,m*2)-m;
}
//angle normalize (-pi, pi]
inline double angn(double t){
	return t-ceil(t/TAU-.5)*TAU;
}

//( -b +- sqrt(b^2-4ac) )/ 2a
inline double quadratic(double a, double b, double c){
	double const srt_det= sqrt(b*b - 4*a*c);
	double const denom= 1./(2*a);
	double A= ( -b - srt_det )*denom;
	double B= ( -b + srt_det )*denom;
	if(A!=A)
		A= 0;
	if(B!=B)
		B= 0;
	return max(A,B);
}
inline double quadratic_ac(double a, double c){
	//c will be negative in kinematic situations
	return -sqrt(c/a);
}
inline double quadratic_ab(double a, double b){
	return -b/a;
}

inline float invsqrt(){
	ass(false);
	return 0;
}

extern thread_local hash_t frand_seed;
inline double frand(){
	frand_seed= hash(frand_seed);
	return ((float)frand_seed)/HASH_MAX;
}
inline double sfrand(){
	return frand()*2.f-1.f;
}

inline float  fract(float x){  return x-(long)x; }
inline double fract(double x){ return x-(long)x; }

struct rati{
	int num, den;

	operator int() const{ return num/den; }

	rati rcp() const{ return {den,num};	}
	rati operator-() const{	return {-num,den}; }
	rati operator*(rati const& x) const{ return {num*x.num, den*x.den};	}
	rati operator/(rati const& x) const{ return {num*x.den, den*x.num}; }
	rati operator+(rati const& x) const{
		//a/b + c/d
		//ad/bd + cb/db
		//ad+cb / bd
		const int a= num;
		const int b= den;
		const int c= x.num;
		const int d= x.den;
		return {a*d+b*c, b*d};
	}
	rati operator-(rati const& x) const{
		const int a= num;
		const int b= den;
		const int c= x.num;
		const int d= x.den;
		return {a*d-b*c, b*d};
	}

	rati operator+(int const& x) const{	return {num+x*den, den}; }
	rati operator-(int const& x) const{	return {num-x*den, den}; }
	rati operator*(int const& x) const{	return {x*num,den};	}
	rati operator/(int const& x) const{	return {num,den*x};	}
};
inline int trunc(rati const& r){ return (int)r; }

struct fixed{
	i64 n;
	static constexpr i64 ZERO= 0ull;
	static constexpr i64 ONE= 1ull<<31;
	static constexpr i64 ALMOST_ONE= ONE-1;
	static constexpr i64 MINVAL= (1ull<<63)+1;
	static constexpr i64 MAXVAL= MINVAL-1;

	constexpr fixed(i64 n_):  n( n_ ){}
	constexpr fixed(ui64 n_): n( n_ ){}
	constexpr fixed(int i):     n( i ){}
	constexpr fixed(double d):  n( d*ONE ){}
	constexpr fixed(rati r):    n( (r.num<<31)/r.den ){}

	fixed operator+(i64 x){ return n+(x<<31); }
	fixed operator-(i64 x){ return n-(x<<31); }
	fixed operator*(i64 x){ return n*x; }
	fixed operator/(i64 x){ return n/x; }

	fixed operator+(fixed x){ return n+x.n; }
	fixed operator-(fixed x){ return n-x.n; }
	//n = g(x,y)*k = f(x*k,y*k)
	fixed operator*(fixed x){
		return 
			 n*(x.n>>31)+//upper
		   ((n*(x.n&ALMOST_ONE))>>31);//lower
	}
	fixed operator/(fixed x){
		return 
			 n/(x.n>>31)+
		   ((n/(x.n&ALMOST_ONE))>>31);
	}
};
inline i32 trunc(fixed x){
	return x.n>>31;
}
inline i32 fract(fixed x){
	return x.n&fixed::ALMOST_ONE;
}

struct cmplx{
	float r,i;
	inline cmplx operator+(cmplx& z) const{
		return {r+z.r, i+z.i};
	}
	inline cmplx operator*(cmplx& z) const{
		const float ar= r, ai= i, br=z.r, bi=z.i;
		return {
			ar*br - ai*bi,
			ar*bi + ai*br
		};
	}
};

}