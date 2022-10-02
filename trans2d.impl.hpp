#include "2d.hpp"

namespace doot{

void mat3x2::operator=(mat3x2 const& b){
	mxx= b.mxx; mxy= b.mxy; tx= b.tx;
	myx= b.myx; myy= b.myy; ty= b.ty;
}

void mat3x2::ident(){
	mxx= 1; mxy= 0; tx= 0;
	myx= 0; myy= 1; ty= 0;
}
void mat3x2::trans(float tx_, float ty_){
	mxx= 1; mxy= 0; tx= tx_;
	myx= 0; myy= 1; ty= ty_;
}
void mat3x2::trans(vec2 t){
	trans(t.x,t.y);
}
void mat3x2::rot(float t){
	trans_rot_scl(0,0,t,1,1);
}
void mat3x2::trans_rot(float tx_, float ty_, float theta_){
	trans_rot_scl(tx_,ty_,theta_,1,1);
}
void mat3x2::trans_rot(vec2 t, float theta_){
	trans_rot_scl(t.x,t.y,theta_,1,1);
}
void mat3x2::trans_rot_scl(vec2 t, float theta, vec2 s){
	trans_rot_scl(t.x,t.y, theta, s.x, s.y);
}
void mat3x2::trans_rot_scl(float tx_, float ty_, float theta, float sx, float sy){
	//z= cos + isin
	float zx= cosf(theta);
	float zy= sinf(theta);
	// z*n= (zx+izy)*(nx+iny)
	//    =  zx*nx + zx*iny + izy*nx + izy*iny
	//    = (zx*nx - zy*ny) + i*(zx*ny + zy*nx)
	// x= zx*nx - zy*ny
	// y= zy*nx + zx*ny
	mxx= zx; mxy=-zy;
	myx= zy; myy= zx;

	mxx*=sx; mxy*=sy;
	myx*=sx; myy*=sy;

	tx= tx_;
	ty= ty_;
}
vec2 mat3x2::operator*(vec2 const& p) const{
	return {
		p.x*mxx + p.y*mxy + tx,
		p.x*myx + p.y*myy + ty
	};
}
vec2 mat3x2::mul_atrans(vec2 const& p) const{
	return {
		p.x*mxx + p.y*mxy,
		p.x*myx + p.y*myy
	};
}
mat3x2 mat3x2::operator*(mat3x2 const& b) const{
	mat3x2 ret;
	ret.mxx= mxx*b.mxx + mxy*b.myx;
	ret.mxy= mxx*b.mxy + mxy*b.myy;
	ret.myx= myx*b.mxx + myy*b.myx;
	ret.myy= myx*b.mxy + myy*b.myy;
	ret.tx=  mxx*b.tx  + mxy*b.ty + tx;
	ret.ty=  myx*b.tx  + myy*b.ty + ty;
	return ret;
}

void mat3x2::inverse_trans_rot_scl(vec2 t, float r, vec2 s){
	float zx= cosf(-r);
	float zy= sinf(-r);
	/*
	[sx  0] * [zx -zy]
	[ 0 sy]   [zy  zx]
	*/
	mxx= zx/s.x; mxy=-zy/s.x;
	myx= zy/s.y; myy= zx/s.y;

	/*
	[xx xy 0] * [1  0 -tx]
	[yx yy 0]   [0  1 -ty]
	[ 0  0 1]   [0  0  1 ]
	*/
	tx= -(t.x*mxx + t.y*mxy);
	ty= -(t.x*myx + t.y*myy);
}

void mat3x2::colMajor(float ret[6]){
	ret[0]= mxx; ret[2]= mxy; ret[4]= tx;
	ret[1]= myx; ret[3]= myy; ret[5]= ty;
}


void mat3x2::unit(vec2& r) const{
	r.x= mxx+mxy+tx;
	r.y= myx+myy+ty;
};
void mat3x2::unit_box(vec2(&r)[4]) const{
	r[0].x= -mxx-mxy+tx;//nn
	r[0].y= -myx-myy+ty;
	r[1].x= -mxx+mxy+tx;//np
	r[1].y= -myx+myy+ty;
	r[2].x= +mxx-mxy+tx;//pn
	r[2].y= +myx-myy+ty;
	r[3].x= +mxx+mxy+tx;//pp
	r[3].y= +myx+myy+ty;
};

}