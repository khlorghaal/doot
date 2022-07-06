#pragma once

namespace doot{

//domain[0,1]
//lerps between entries
//default fits into a cache line
//a low order transcendental may be better
template<typename T= float, unsigned int slots= 128, bool circular= false>
class lerplut{
	T s[slots+1];//an overflow slot duplicating the last, because bi-interpolation
public:
	flut( T(*f)(T) ){
		for(int i=0; i!=slots; i++)
			s[i]= f( float(i)/(slots-1) );
	};
	s[slots]= s[slots-1];//overflow slot

	inline operator[](T t);
};


template<int s>
float lerplut<float, s, false>::operator[](float x){
	unsigned int const i= x*slots;
	float a= s[i  ];
	float b= s[i+1];
	float t= (x*slots)-i;//floor
	return (b-a)*t + a;//lerp
};

}