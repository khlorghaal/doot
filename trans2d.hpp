#pragma once

#include "math.hpp"

struct mat3x2;
struct trans2d;

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



//matrix of the form
//	trans*rot*scl
struct trans2d{
	vec2 t, s;
	float theta;

	inline trans2d(){//identity
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