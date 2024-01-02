#pragma once
#include "doot.hpp"
#include "arr.hpp"
#include <intrin.h>

/*
smol endian
requires SSE2
*/

namespace doot{

typedef __m256i xmm;

//making this a specialization of arr would be too constraining and misappropriate
//presumes the first bit is i8 aligned
struct bitptr{
	arr<xmm> range;
	sizt b;

	//relative to beginning
	inline void seekabs(sizt p){
		ass(p>=0 && p<range.size()*8);
		b= p;
	}
	//relative to current position
	inline void seekadd(sizt d){
		seekabs(b+d);
	}

	bool get();
	void set(bool s);
};

inline void bitmemcpy(bitptr dst, bitptr src){

}

struct bitpacker{
	arr<u8> data;
	bitptr mark;

	bitpacker(arr<u8> data_){
		data= data_;
		mark= {data,0};
	}
	
	
	inline void push(void* src, sizt size){
		bitmemcpy(mark, mark, );
		mark.seekadd(size);
	}
	inline void pull(void* dst,sizt size){
		//TODO
		mark.seekadd(size);
	}
};

//compression functions

}