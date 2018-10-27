#pragma once

typedef unsigned int hash_t;
#define HASH_MAX 0xFFFFFFFFUL

//TODO crc intrinsics

inline hash_t hash(unsigned int x){
	hash_t h= 1;
	for(int i=0; i!=4; i++){
		h+= *(i+&x);
		h= (h<<6)+(h<<16)-h;
		h*= 0x7FFFFFFF;
	}
	return h;
}
inline hash_t hash(unsigned long long x){
	hash_t h= 1;
	for(int i=0; i!=8; i++){
		h+= *(i+&x);
		h= (h<<6)+(h<<16)-h;
		h*= 0x7FFFFFFF;
	}
	return h;
}
inline hash_t hash(void* x){     return hash((unsigned long long) x); };
inline hash_t hash(int   x){     return hash((unsigned int)       x); };
inline hash_t hash(long long x){ return hash((unsigned long long) x); };
extern hash_t hash(char const*);//string.cpp