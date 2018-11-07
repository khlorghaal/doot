#pragma once
#include "global.hpp"
#include "vec2.hpp"
#include "trans2d.hpp"
#include "hash.hpp"
#include <cmath>
#undef min
#undef max
#define NOMINMAX /*windows.h*/

namespace doot{
using ::sqrt;
using ::sin;
using ::cos;
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
constexpr double ETA= 1.e-12;
const double SRT2= sqrt(2.);
const double SRT2INV= (1./SRT2);

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

inline double mod(double x, double m){ return fmod(x,m); }
inline float mod(float x, float m){ return fmodf(x,m); }

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
#define lerp_field(x_) ret.x_= lerp(t, a.x_, b.x_)
inline mat3x2 lerp(float t, mat3x2 const& a, mat3x2 const& b){
	mat3x2 ret;
	lerp_field(mxx);
	lerp_field(mxy);
	lerp_field(myx);
	lerp_field(myy);
	lerp_field(tx);
	lerp_field(ty);
	return ret;
}
inline trans2d lerp(float t, trans2d const& a, trans2d const& b){
	trans2d ret;
	lerp_field(t);
	lerp_field(s);
	lerp_field(theta);
	return ret;
}

template<typename T>
inline float lerp(float t, gvec2<T> const& a, gvec2<T> const& b){
	return {
		lerp_field(x);
		lerp_field(y);
	};
}

template<typename T>
inline T perp(float const x, T const a, T const b, T const c){
	T ca;
	T cb;
	T cc;
	assert(false);
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
//angle normalize
inline double angn(double t){
	return fmod(t+PI,TAU)-PI;
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
	assert(false);
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

template<typename T> inline T len( gvec2<T> const& a){ return sqrt(a.x*a.x + a.y*a.y); }
template<typename T> inline gvec2<T> norm(gvec2<T> const& a){ return a/len(a); }
template<typename T> inline double angle(gvec2<T> const& a){  return atan2((double)a.y,(double)a.x); }

template<typename T> inline T dot(  gvec2<T> const& a, gvec2<T> const& b){ return a.x*b.x + a.y*b.y; }
template<typename T> inline gvec2<T> rot4(gvec2<T> const& a){ return { a.y,-a.x }; }

inline float  fract(float x){  return x-(long)x; }
inline double fract(double x){ return x-(long)x; }

template<typename T> inline gvec2<T> sign( gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( x>0?1:x<0?-1:0, y>0?1:y<0?-1:0); }
template<typename T> inline gvec2<T> fract(gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( fract(x),fract(y) ); }
template<typename T> inline gvec2<T> floor(gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( floor(x),floor(y) ); }
template<typename T> inline gvec2<T> ciel( gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( ciel(x),ciel(y) ); }
template<typename T> inline gvec2<T> sin(  gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( sin(x),sin(y) ); }
template<typename T> inline gvec2<T> cos(  gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( cos(x),cos(y) ); }
template<typename T> inline gvec2<T> sqrt( gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( sqrt(x),sqrt(y) ); }
template<typename T> inline gvec2<T> log(  gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( log(x),log(y) ); }
template<typename T> inline gvec2<T> log2( gvec2<T> a){ T const& x= a.x, y= a.y; return gvec2<T>( log2(x),log2(y) ); }

template<typename T> inline gvec2<T> pow(gvec2<T> a, T p){ T const& x= a.x, y= a.y; return gvec2<T>( pow(x,p),pow(y,p) ); }
template<typename T> inline gvec2<T> mod(gvec2<T> a, T m){ T const& x= a.x, y= a.y; return gvec2<T>( mod(x,m),mod(y,m) ); }

template<typename T>
inline charstream& operator<<(charstream& c, gvec2<T> v){
	c<<string::fmt("[%.4f,%.4f]",v.x,v.y);
	return c;
}

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
template<typename T>
inline charstream& operator<<(charstream& c, rati const& r){
	c<<string::fmt("[%i/%i]",r.num,r.den);
	return c;
}

struct fixed{
	int64 n;
	static constexpr int64 ZERO= 0ull;
	static constexpr int64 ONE= 1ull<<31;
	static constexpr int64 ALMOST_ONE= ONE-1;
	static constexpr int64 MINVAL= (1ull<<63)+1;
	static constexpr int64 MAXVAL= MINVAL-1;

	constexpr fixed(int64 n_):  n( n_ ){}
	constexpr fixed(uint64 n_): n( n_ ){}
	constexpr fixed(int i):     n( i<<31 ){}
	constexpr fixed(double d):  n( d*ONE ){}
	constexpr fixed(rati r):    n( (r.num<<31)/r.den ){}

	fixed operator+(int64 x){ return n+(x<<31); }
	fixed operator-(int64 x){ return n-(x<<31); }
	fixed operator*(int64 x){ return n*x; }
	fixed operator/(int64 x){ return n/x; }

	fixed operator+(fixed x){ return n+x.n; }
	fixed operator-(fixed x){ return n-x.n; }
	//n = g(x,y)*k = f(x*k,y*k)
	fixed operator*(fixed x){
		return 
			 n*(x.n>>31)+//upper
			(n*(x.n&ALMOST_ONE))>>31;//lower
	}
	fixed operator/(fixed x){
		return 
			 n/(x.n>>31)+
			(n/(x.n&ALMOST_ONE))>>31;
	}
};
inline int32 trunc(fixed x){
	return x.n>>31;
}
inline int32 fract(fixed x){
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