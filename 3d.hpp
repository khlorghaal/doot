/*
basis vectors: x horizon-right, y up, z forward
*/
#pragma once
#include "math.hpp"

namespace doot{

struct vec3{
	float x,y,z;
};
struct mat4{
	float
	mxx,mxy,mxz,mxw,
	myx,myy,myz,myw,
	mzx,mzy,mzz,mzw,
	mwx,mwy,mwz,mww;

	inline operator float*(){ return &mxx; }
};

struct quat{
	float x,y,z,w;
	inline void operator*=(quat b){  }
};
//up is level to y
inline quat look(vec3 v){
	return {};
}
inline quat look(vec3 fwd, vec3 up){
	return {};
}

struct cam{
	vec3 pos;
	quat look;
	float fov_deg;

	mat4 view();
	mat4 viewprojection();
};

}