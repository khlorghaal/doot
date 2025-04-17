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
struct mat2x2;
struct mat3x2;
struct trans2;
struct trans2ch;


tplt struct gvec2{
	using type= T;
	T x,y;
	gvec2(){ x= y= 0;};
	//explicit needed since c++ preceden't ctor declaratively
	                	explicit gvec2(T   a       ){ x=a     ; y=a     ; };
	                	         gvec2(T   a, T   b){ x=a     ; y=b     ; };
	tpl<typn A, typn B> explicit gvec2(A   a, B   b){ x=(T)a  ; y=(T)b  ; };
	tple                explicit gvec2(gvec2<E> a  ){ x=(T)a.x; y=(T)a.y; };
	tple                inl gvec2<T> op=(E a       ){ x=(T)a  ; y=(T)a  ; reth; };
	tple                inl gvec2<T> op=(gvec2<E> a){ x=(T)a.x; y=(T)a.y; reth; };
	//#define L(V) 
	//VTYP_RX;
	//#undef L

	//list
	inl gvec2<T> op+(gvec2<T> a) cst { re gvec2<T>(x+a.x, y+a.y); }
	inl gvec2<T> op-(gvec2<T> a) cst { re gvec2<T>(x-a.x, y-a.y); }
	inl gvec2<T> op*(gvec2<T> a) cst { re gvec2<T>(x*a.x, y*a.y); }
	inl gvec2<T> op/(gvec2<T> a) cst { re gvec2<T>(x/a.x, y/a.y); }

	inl gvec2<T>& op+=(gvec2<T> a){ x+=a.x, y+=a.y; reth; }
	inl gvec2<T>& op-=(gvec2<T> a){ x-=a.x, y-=a.y; reth; }
	inl gvec2<T>& op*=(gvec2<T> a){ x*=a.x, y*=a.y; reth; }
	inl gvec2<T>& op/=(gvec2<T> a){ x/=a.x, y/=a.y; reth; }

	inl gvec2<T> op-() cst { re gvec2<T>(-x,-y); }

	//scalar
	tple inl gvec2<T> op+(E a) cst { re gvec2<T>(x+a,y+a); }
	tple inl gvec2<T> op-(E a) cst { re gvec2<T>(x-a,y-a); }
	tple inl gvec2<T> op*(E a) cst { re gvec2<T>(x*a,y*a); }
	tple inl gvec2<T> op/(E a) cst { re gvec2<T>(x/a,y/a); }

	tple inl gvec2<T>& op+=(E a){ x+=a; y+=a; reth; }
	tple inl gvec2<T>& op-=(E a){ x-=a; y-=a; reth; }
	tple inl gvec2<T>& op*=(E a){ x*=a; y*=a; reth; }
	tple inl gvec2<T>& op/=(E a){ x/=a; y/=a; reth; }

	tpl<typn A> inl bool op==(A a){ re a.x==x && a.y==y; }
	tpl<typn A> inl bool op!=(A a){ re a.x!=x || a.y!=y; }

	//tple op gvec2<E>(){ re gvec2<E>(x,y);}

	op str() cst{ re "v["_s+x+","+y+"]"; }
};

using   vec2= gvec2<f32>;
using  dvec2= gvec2<f64>;
using  ivec2= gvec2<i32>;
using  lvec2= gvec2<i64>;
using  uvec2= gvec2<u32>;
using ulvec2= gvec2<u64>;

#define vec2_kinds vec2,dvec2,ivec2,lvec2,uvec2,ulvec2

//inl f64 abs(f64 a){ re ::doot::abs((f64)a); }
//inl f32 abs(f32 a){ re ::doot::abs((f32)a); }
//inl i32 abs(i32 a){ re ::doot::abs((i32)a); }
//inl vec2 abs(vec2 a){ re vec2(::doot::abs((f32)a.x),::doot::abs((f32)a.y)); }
//inl dvec2 abs(dvec2 a){ re dvec2(::doot::abs((f64)a.x),::doot::abs((f64)a.y)); }
//inl ivec2 abs(ivec2 a){ re ivec2(::doot::abs((i32)a.x),::doot::abs((i32)a.y)); }

//needed as template overload causes issues with non-template scalar declarations
//lval helps coercion issues
#define vec2ize_1_(F,V) \
	inl V F(V a){ V r; r.x= F((V::type)a.x); r.y= F((V::type)a.y); retr; }
#define vec2ize_2_(F,V) \
	inl V F(V       a,V       b){ using T= V::type; V r; r.x= F((T)a.x,(T)b.x); r.y= F((T)a.y,(T)b.y); retr; }\
	inl V F(V       a,V::type b){ using T= V::type; V r; r.x= F((T)a.x,(T)b  ); r.y= F((T)a.y,(T)b  ); retr; }\
	inl V F(V::type a,V       b){ using T= V::type; V r; r.x= F((T)a  ,(T)b.x); r.y= F((T)a  ,(T)b.y); retr; }
#define vec2ize_1f(F) vec2ize_1_(F,vec2) vec2ize_1_(F,dvec2)
#define vec2ize_2f(F) vec2ize_2_(F,vec2) vec2ize_2_(F,dvec2)
#define vec2ize_1(F)  vec2ize_1f(F)      vec2ize_1_(F,ivec2) vec2ize_1_(F,lvec2)
#define vec2ize_2(F)  vec2ize_2f(F)      vec2ize_2_(F,ivec2) vec2ize_2_(F,lvec2)

vec2ize_1f(fract);
vec2ize_1f(floor);
vec2ize_1f( ceil);
vec2ize_1f(  sin);
vec2ize_1f(  cos);
vec2ize_1f( sqrt);

vec2ize_1(  abs);
vec2ize_1( sign);
vec2ize_1f(  log);
vec2ize_1f( log2);

vec2ize_2f(mod);
vec2ize_2f(smod);

vec2ize_2f(pow);
vec2ize_2(min);
vec2ize_2(max);

inl f32 min( vec2 v){ re min(v.x,v.y); }
inl f32 max( vec2 v){ re max(v.x,v.y); }
inl f64 min(dvec2 v){ re min(v.x,v.y); }
inl f64 max(dvec2 v){ re max(v.x,v.y); }

inl  vec2 lerp( vec2 x,  vec2 a,  vec2 b){ re  vec2(lerp(x.x,a.x,b.x),lerp(x.y,a.y,b.y)); }
inl dvec2 lerp(dvec2 x, dvec2 a, dvec2 b){ re dvec2(lerp(x.x,a.x,b.x),lerp(x.y,a.y,b.y)); }
inl  vec2 lerp(  f32 t,  vec2 a,  vec2 b){
	re vec2(
		lerp(t,a.x,b.x),
		lerp(t,a.y,b.y)
	);
}
//unary
tplt inl T sum( gvec2<T> cre a){ re a.x+a.y; }
tplt inl T prod(gvec2<T> cre a){ re a.x*a.y; }
tplt inl T len(gvec2<T> cre a){ re sqrt(sum(a*a)); }
tplt inl gvec2<T> norm(gvec2<T> cre a){ re a/len(a); }//todo invsqrt
tplt inl f64 atan(gvec2<T> cre a){ re atan2((f64)a.y,(f64)a.x); }
tplt inl gvec2<T> trot(gvec2<T> cre a){ re {-a.y,a.x}; }
//todo div safety

tplt inl T dot(gvec2<T> cre a,gvec2<T> cre b){ re a.x*b.x + a.y*b.y; }
tplt inl gvec2<T> rot4(gvec2<T> cre a){ re {a.y,-a.x}; }


struct ray{
	vec2 a;//span, or normal
	vec2 c;//origin
};
struct ray_fin: ray{};//finite, c is end
struct ray_inf: ray{};//infinite, c is normal
/*
struct line{
	f32 a,b,c;//0 = ax + by + c
	line(){
		#ifdef DEBUG
		a=b=c= NAN;
		#endif
	}
	line(f32 a_,f32 b_,f32 c_){ a= a_; b= b_; c= c_; };
	line(ray r){
		ass(false);//TODO
	}
	//distance to origin
	f32 dist0(){
		re abs(c)/len(vec2{a,b});
	}
};
//(a[x|y]+c)/-b = c = [y|x] = 0
cst line LINE_X={0,1,0};
cst line LINE_Y={1,0,0};
*/

struct point{
	vec2 p;
};
struct circle{
	vec2 p;
	f32 r;
};
struct mat2x2{
	f32 mxx=1, mxy=0,
	    myx=0, myy=1;

	op str() cst{ re str(arr<f32 cst>{&mxx,(&myy)+1}); };
};
struct mat3x2{
	f32 mxx=1, mxy=0, tx=0,
	    myx=0, myy=1, ty=0;

	void op=(mat3x2 cre);

	//assignment
	void ident();
	void trans(f32 tx, f32 ty);
	void trans(vec2 t);
	void rot(f32 theta);
	void trans_rot(f32 tx, f32 ty, f32 theta);
	void trans_rot(vec2 t, f32 theta);
	void trans_rot_scl(f32 tx, f32 ty, f32 theta, f32 sx, f32 sy);
	void trans_rot_scl(vec2 t, f32 theta, vec2 s);
	void inverse_trans_rot_scl(vec2 t, f32 r, vec2 s);//scale^-1 * rot^-1 * trans^-1

	vec2 op*(vec2 cre p) cst;
	vec2 mul_atrans(vec2 cre) cst;//multiply without translation
	mat3x2 op*(mat3x2 cre) cst;
	
	//these elide all multiplications
	inl vec2 unitX()  cst{ re {mxx    +tx,myx    +ty}; };//mul by [1,0]
	inl vec2 unit11() cst{ re {mxx+mxy+tx,myx+myy+ty}; };//mul by [1,1]
	void unit_box(vec2(&)[4]) cst;//multiply by vec2 permutations [-1,1]

	op mat2x2() cst{ re {
		mxx, mxy,
		myx, myy }; }
	op str() cst{ re str(arr<f32 cst>{&mxx,(&ty)+1}); };
};
inl mat3x2 lerp(f32 t, mat3x2 cre a, mat3x2 cre b){
	re {
		lerp_ab(mxx),lerp_ab(mxy),lerp_ab(tx),
		lerp_ab(myx),lerp_ab(myy),lerp_ab(ty)
	};
}


//convert angle to normal
//eschew for precomputed matrix-unit
inl vec2 rotnorm2(f32 theta){
	re vec2(cos(theta),sin(theta));
}
//matrix of the form
//	trans*rot*scl
struct trans2{
	vec2 t, s;
	rad theta=0;

	mat3x2 mat;
	mat3x2 matinv;

	trans2()= default;//identity
	trans2(vec2 t_, vec2 s_,    rad th_):
		      t(t_),   s(s_), theta(th_){}

	inl op mat3x2() cst{ re mat; };
	//inl op rect() cst{ re {mat,mat_inverse,s}; }

	inl vec2 rotate(vec2 in){ mat3x2 m; m.rot(theta); re m*in; }

	void clean(){//update matrix
		mat.           trans_rot_scl(t, theta, s);
		matinv.inverse_trans_rot_scl(t, theta, s);
	};

	op str() cst{ re "tr:"_s+t.op str()+",s:"+s.op str()+",th:"+theta+";"; }
};
inl trans2 lerp(f32 t,trans2 cre a,trans2 cre b){
	trans2 r;
	r.t    = lerp(t, a.t,     b.t);
	r.s    = lerp(t, a.s,     b.s);
	r.theta= lerp(t, a.theta, b.theta);
	r.clean();
	retr;
}

//child transform
//does not inherit scale, because reasons
struct trans2ch{
	trans2 local;
	trans2 global;
	trans2ch(){}
	trans2ch(trans2 l, trans2 p): local(l), global(){
		update(p);
	}
	//updates global from parameter
	//does not mutate local
	void update(trans2 cre parent){
		mat3x2 p= parent.mat;
		global.t= p*local.t;
		global.s= local.s;
		global.theta= local.theta+parent.theta;
	};
};




}