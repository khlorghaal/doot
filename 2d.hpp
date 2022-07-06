#pragma once
#include "primitives.hpp"
#include "math.hpp"

// 0 theta = {1,0}; CCW+

namespace doot{

struct ray;
struct ray_fin;
struct ray_inf;
struct line;
struct point;
struct rect;
struct circle;
struct mat3x2;
struct trans2;
struct trans2ch;



template<typename T>
struct gvec2{
	T x,y;
	gvec2(          ){ x= y= 0;};
	gvec2(T x_      ){ x=x_; y=x_; };
	gvec2(T x_, T y_){ x=x_; y=y_; };
	template<typename C>
	gvec2(gvec2<C> c){ x=(T)c.x; y=(T)c.y; };

	template<typename C>
	inline gvec2<T> operator=(gvec2<C> const& c){ x=c.x; y=c.y; }

	//vector
	inline gvec2<T> operator+(gvec2<T> const a) const { return gvec2<T>(x+a.x, y+a.y); }
	inline gvec2<T> operator-(gvec2<T> const a) const { return gvec2<T>(x-a.x, y-a.y); }
	inline gvec2<T> operator*(gvec2<T> const a) const { return gvec2<T>(x*a.x, y*a.y); }
	inline gvec2<T> operator/(gvec2<T> const a) const { return gvec2<T>(x/a.x, y/a.y); }

	inline gvec2<T>& operator+=(gvec2<T> const a){ x+=a.x, y+=a.y; return *this; }
	inline gvec2<T>& operator-=(gvec2<T> const a){ x-=a.x, y-=a.y; return *this; }
	inline gvec2<T>& operator*=(gvec2<T> const a){ x*=a.x, y*=a.y; return *this; }
	inline gvec2<T>& operator/=(gvec2<T> const a){ x/=a.x, y/=a.y; return *this; }

	inline gvec2<T> operator-() const { return gvec2<T>(-x,-y); }

	//scalar
	template<typename A> inline gvec2<T> operator+(A a) const { return gvec2<T>(x+a,y+a); }
	template<typename A> inline gvec2<T> operator-(A a) const { return gvec2<T>(x-a,y-a); }
	template<typename A> inline gvec2<T> operator*(A a) const { return gvec2<T>(x*a,y*a); }
	template<typename A> inline gvec2<T> operator/(A a) const { return gvec2<T>(x/a,y/a); }

	template<typename A> inline gvec2<T>& operator+=(A a){ x+=a; y+=a; return *this; }
	template<typename A> inline gvec2<T>& operator-=(A a){ x-=a; y-=a; return *this; }
	template<typename A> inline gvec2<T>& operator*=(A a){ x*=a; y*=a; return *this; }
	template<typename A> inline gvec2<T>& operator/=(A a){ x/=a; y/=a; return *this; }

	template<typename A> inline bool operator==(A a){ return a.x==x && a.y==y; }
	template<typename A> inline bool operator!=(A a){ return a.x!=x || a.y!=y; }
};

typedef gvec2<float>    vec2;
typedef gvec2<double>  dvec2;
typedef gvec2<i32>   ivec2;
typedef gvec2<i64>   lvec2;
typedef gvec2<ui32> uivec2;
typedef gvec2<ui64> ulvec2;
template<typename T>
inline gvec2<T> lerp(float t,gvec2<T> const& a,gvec2<T> const& b){
	return {
		lerp(t, a.x, b.x),
		lerp(t, a.y, b.y)
	};
}
//unary
template<typename T> inline T len(gvec2<T> const& a){ return sqrt(a.x*a.x + a.y*a.y); }
template<typename T> inline gvec2<T> norm(gvec2<T> const& a){ return a/len(a); }
template<typename T> inline double angle(gvec2<T> const& a){ return atan2((double)a.y,(double)a.x); }

template<typename T> inline T dot(gvec2<T> const& a,gvec2<T> const& b){ return a.x*b.x + a.y*b.y; }
template<typename T> inline gvec2<T> rot4(gvec2<T> const& a){ return {a.y,-a.x}; }

template<typename T> inline T min(gvec2<T> a){ T const& x= a.x,y= a.y; return min(x,y); }
template<typename T> inline T max(gvec2<T> a){ T const& x= a.x,y= a.y; return max(x,y); }
template<typename T> inline gvec2<T> min(gvec2<T> a,gvec2<T> b){ return {min(a.x,b.x),min(a.y,b.y)}; }
template<typename T> inline gvec2<T> max(gvec2<T> a,gvec2<T> b){ return {max(a.x,b.x),max(a.y,b.y)}; }

//scalar n-ary
template<typename T> inline gvec2<T> abs(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(abs(x),abs(y)); }
template<typename T> inline gvec2<T> sign(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(x>0?1:x<0?-1:0,y>0?1:y<0?-1:0); }
template<typename T> inline gvec2<T> fract(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(fract(x),fract(y)); }
template<typename T> inline gvec2<T> floor(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(floor(x),floor(y)); }
template<typename T> inline gvec2<T> ciel(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(ciel(x),ciel(y)); }
template<typename T> inline gvec2<T> sin(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(sin(x),sin(y)); }
template<typename T> inline gvec2<T> cos(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(cos(x),cos(y)); }
template<typename T> inline gvec2<T> sqrt(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(sqrt(x),sqrt(y)); }
template<typename T> inline gvec2<T> log(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(log(x),log(y)); }
template<typename T> inline gvec2<T> log2(gvec2<T> a){ T const& x= a.x,y= a.y; return gvec2<T>(log2(x),log2(y)); }
template<typename T> inline gvec2<T> pow(gvec2<T> a,T p){ T const& x= a.x,y= a.y; return gvec2<T>(pow(x,p),pow(y,p)); }
template<typename T> inline gvec2<T> mod(gvec2<T> a,T m){ T const& x= a.x,y= a.y; return gvec2<T>(mod(x,m),mod(y,m)); }






struct ray{
	vec2 a;//span, or normal
	vec2 c;//origin
};
struct ray_fin: ray{};//finite, c is end
struct ray_inf: ray{};//infinite, c is normal
struct line{
	float a,b,c;//0 = ax + by + c
	inline line(){
		#ifdef DEBUG
		a=b=c= NAN;
		#endif
	}
	inline line(float a_,float b_,float c_){ a= a_; b= b_; c= c_; };
	inline line(ray r){
		ass(false);//TODO
	}
	//distance to origin
	inline float dist0(){
		return abs(c)/len(vec2{a,b});
	}
};
//(a[x|y]+c)/-b = c = [y|x] = 0
const line LINE_X={0,1,0};
const line LINE_Y={1,0,0};
struct point{
	vec2 p;
};
struct circle{
	vec2 p;
	float r;
};
struct mat3x2{
	float mxx, mxy, tx;
	float myx, myy, ty;

	mat3x2()= default;
	void operator=(mat3x2 const&);

	//assignment
	void ident();
	void trans(float tx, float ty);
	void trans(vec2 t);
	void trans_rot(float tx, float ty, float theta);
	void trans_rot(vec2 t, float theta);
	void trans_rot_scl(float tx, float ty, float theta, float sx, float sy);
	void trans_rot_scl(vec2 t, float theta, vec2 s);
	void rot(float theta);
	static mat3x2 inverse_trans_rot_scl(vec2 t, float r, vec2 s);//scale^-1 * rot^-1 * trans^-1

	vec2 operator*(vec2 const& p) const;
	vec2 mul_atrans(vec2 const&) const;//multiply without translation
	mat3x2 operator*(mat3x2 const&) const;

	void matrix_colMajor(float ret[6]);

	//these omit all multiplications
	void unit(vec2&) const;//multiply by [1,1]
	void unit_box(vec2(&)[4]) const;//multiply by vec2 permutations [-1,1]
};
inline mat3x2 lerp(float t,mat3x2 const& a,mat3x2 const& b){
	mat3x2 ret;
	lerp_field(mxx);
	lerp_field(mxy);
	lerp_field(myx);
	lerp_field(myy);
	lerp_field(tx);
	lerp_field(ty);
	return ret;
}
struct rect{
	mat3x2 mat,inv;
	vec2 wh;
};
//convert angle to normal
//eschew in favor of {m.xx,m.yx}
inline vec2 rotnorm2(float theta){
	return {cosf(theta),sinf(theta)};
}
//matrix of the form
//	trans*rot*scl
struct trans2{
	vec2 t, s;
	float theta;

	trans2(){//identity
		t= 0;
		s= 1;
		theta= 0;
	};
	trans2(vec2 t_, vec2 s_, float th_): t(t_), s(s_), theta(th_){}
	mat3x2 mat() const{
		mat3x2 ret;
		ret.trans_rot_scl(t, theta, s);
		return ret;	
	};
	inline operator mat3x2() const{ return mat(); };
	inline operator rect() const{ return {mat(),mat_inverse(),s}; }

	inline vec2 rotate(vec2 in){ mat3x2 m; m.rot(theta); return m*in; }

	inline mat3x2 mat_inverse() const{
		return mat3x2::inverse_trans_rot_scl(t,theta,s);
	};
};
inline trans2 lerp(float t,trans2 const& a,trans2 const& b){
	trans2 ret;
	lerp_field(t);
	lerp_field(s);
	lerp_field(theta);
	return ret;
}

//child transform
//does not inherit scale
struct trans2ch{
	trans2 local;
	trans2 global;
	trans2ch(){}
	trans2ch(trans2 l, trans2 p): local(l), global(){
		update(p);
	}
	//updates global from parameter
	//does not mutate local
	void update(trans2 const& parent){
		mat3x2 p= parent.mat();
		global.t= p*local.t;
		global.s= local.s;
		global.theta= local.theta+parent.theta;
	};
};




}