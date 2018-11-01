#pragma once
#include "global.hpp"

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
