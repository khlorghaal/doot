#pragma once
#include "global.hpp"

struct bitpacker{
	size_t* word;
	char bit;

	inline void incr(unsigned char i){
		bit+= i;
		char const bdiv= bit/64;
		word+= bdiv;
		bit-=  bdiv;

		#ifndef NDEBUG
		if(i>64 || bit<0)
			throw;
		#endif
	}
	
	//overwrites following words
	inline void push(unsigned long long const& src, char srcsize){
		/*
		--| bit index, <wordsize
		xxxx xxxx  current and next words
		      abc  write value, size<=wordsize		  
		c0ab c0ab  shifted >>> offset
		1100       highmask
		     0111  lowmask
		11ab c111  vals | masks
		xxab cxxx  masked vals & result
		*/
		unsigned long long & ml= *(word  );
		unsigned long long & mh= *(word+1);

		unsigned char const dsiz= 64-srcsize;
		         char       shift= bit+srcsize;
		unsigned long long const shifted= src>>>shift;
		unsigned long long  hmask= -1ull<<;
		unsigned long long  lmask= -1ull>>;
		unsigned long long  high= shifted|hmask;
		unsigned long long  low=  shifted|lmask;


		unsigned long long mask= ~(-1ull<<srcsize);
		unsigned long long acc= src;
		char lowfit= 64-bit;
		char lowshift= ;



		ml= ml&low;
		mh= mh;



		incr(srcsize);
	}

	/*
	xxaa axxx two words being read
	0011 1000  lmask, hmask
	00aa a000  result
	*/
	inline void pull(unsigned long long const& dst, char srcsize){


		incr(srcsize);
	}
};

template<size_t cap>
struct bitarr{
	uint8 dat[cap/8+1];
	bool& operator[](size_t b){
		
	}
};