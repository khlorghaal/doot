#pragma once
#include "defs.hpp"
//this is a header so that externs may be refed without including all of doot

namespace doot{

struct voidstruct{};//for changing maps into sets etc

using siz_t= unsigned long long;
using sizt = unsigned long long;
using siz  = unsigned long long;
using i8=   char;
using i16=  short;
using i32=  int;
using i64=  long long;
using  u8= unsigned  char;
using u16= unsigned short;
using u32= unsigned;
using u64= unsigned long long;
using f32=  float;
using f64=  double;
using cstr= char const*;
using fL= long double;//abomination should only be used for cexs

#define null 0
//must define otherwise endless type coercion bitching


using index= u32;
using    id= u32;
using idx= index;
using eid= id;//for namespacing and clarity, as ptr<T>::id isomorph T
//use of distinct types was evited due to potential complexity
//component arrays prefered over subentities to min deref
index cex NULLIDX= -1u  ;  index cex nullidx= NULLIDX;
   id cex NULLID = -1u  ;     id cex nullid = NULLID ;
  siz cex NULLSIZ= -1ull;    siz cex nullsiz= NULLSIZ;
struct idid{
	id e= NULLID;
	id i= NULLID;
	inline bool op!(){        re e==NULLID || i==NULLID; };
	inline bool op==(idid b){ re e==b.e    && i==b.i;    };

	#define NULLIDID {NULLID,NULLID}
};

using hash_t= u32;
hash_t cex HASH_MAX= 0xFFFFFFFFUL;
f32 cex HASH_MAX_F= f32(HASH_MAX);

siz cex TOO_BIG= 0x10000000ULL-1;
//no reasonable allocation allowed to excede

using nsec= i64;//nanoseconds are the standard time unit
//for clarity, do not statefully store or pass other time units
//i tried strongly typed implicit scalar basis conversions, but it was against the will of god
using time_t= i64;

cex nsec op""_ns(u64 v){ re v; }
cex nsec op""_us(u64 v){ re v*1'000ll; }
cex nsec op""_ms(u64 v){ re v*1'000'000ll; }
cex nsec op""_s( u64 v){ re v*1'000'000'000ll; }
cex time_t usec(nsec t){       re t/1'000ll; }
cex time_t msec(nsec t){       re t/1'000'000ll; }
cex time_t  sec(nsec t){       re t/1'000'000'000ll; }

}
