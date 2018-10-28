#pragma once

namespace doot{

typedef unsigned int hash_t;
#define HASH_MAX 0xFFFFFFFFUL

//TODO crc intrinsics

inline hash_t hash(unsigned int x){
	x= ((x>>16)^x)*0x45d9f3bul;
	x= ((x>>16)^x)*0x45d9f3bul;
	x=  (x>>16)^x;
	return x;
}
inline hash_t hash(unsigned long long x){
	x= (x^(x>>30))*0xbf58476d1ce4e5b9ull;
	x= (x^(x>>27))*0x94d049bb133111ebull;
	x=  x^(x>>31);
	return x;
}
inline hash_t hash(void* x){     return hash((unsigned long long) x); };
inline hash_t hash(int   x){     return hash((unsigned int)       x); };
inline hash_t hash(long long x){ return hash((unsigned long long) x); };
extern hash_t hash(char const*);//string.cpp

}