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
//presumes the first bit is byte aligned
struct bitptr{
	arr<xmm> range;
	size_t b;

	//relative to beginning
	inline void seekabs(size_t p){
		assert(p>=0 && p<range.size()*8);
		b= p;
	}
	//relative to current position
	inline void seekadd(size_t d){
		seekabs(b+d);
	}

	bool get();
	void set(bool s);
};

inline void bitmemcpy(bitptr dst, bitptr src){

}

struct bitpacker{
	arr<ubyte> data;
	bitptr mark;

	bitpacker(arr<ubyte> data_){
		data= data_;
		mark= {data,0};
	}
	
	
	inline void push(void* src, size_t size){
		bitmemcpy(mark, mark, );
		mark.seekadd(size);
	}
	inline void pull(void* dst,size_t size){
		//TODO
		mark.seekadd(size);
	}
};

//compression functions

}