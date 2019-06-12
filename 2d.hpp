#pragma once
#include "primitives.hpp"

// 0 theta = {1,0}; CCW+

namespace doot{

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
typedef gvec2<int32>   ivec2;
typedef gvec2<int64>   lvec2;
typedef gvec2<uint32> uivec2;
typedef gvec2<uint64> ulvec2;



struct mat3x2;
struct trans2;

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

	inline trans2(){//identity
		t= 0;
		s= 1;
		theta= 0;
	};
	inline mat3x2 mat() const{
		mat3x2 ret;
		ret.trans_rot_scl(t, theta, s);
		return ret;	
	};
	inline operator mat3x2() const{ return mat(); };

	inline vec2 rotate(vec2 in){ mat3x2 m; m.rot(theta); return m*in; }

	inline mat3x2 mat_inverse() const{
		return mat3x2::inverse_trans_rot_scl(t,theta,s);
	};
};

//child transform
//does not inherit scale
struct trans2ch{
	trans2 local;
	trans2 global;
	void update(trans2& parent){
		mat3x2 p= parent.mat();
		global.t= p*local.t;
		global.s= local.s;
		global.theta= local.theta+parent.theta;
	};
};


}