#pragma once

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
using fL= long double;//abomination should only be used for constexprs

#define null 0
//must define otherwise endless type coercion bitching


using index=  u32;
using idx= index;
using  id=u32;
using eid=u32;//entity ID
using cid=u32;//component ID, where an entity possess multiple components, cids are unique per entity
//use of distinct types was evited due to potential complexity
//component arrays prefered over subentities to min deref
index constexpr NULLIDX= -1u  ;  index constexpr nullidx= NULLIDX;
   id constexpr NULLID = -1u  ;     id constexpr nullid = NULLID ;
  siz constexpr NULLSIZ= -1ull;    siz constexpr nullsiz= NULLSIZ;
struct idid{
	id e= NULLID;
	id i= NULLID;
	inline bool operator!(){        return e==NULLID || i==NULLID; };
	inline bool operator==(idid b){ return e==b.e    && i==b.i;    };

	#define NULLIDID {NULLID,NULLID}
};

using hash_t= u32;
hash_t constexpr HASH_MAX= 0xFFFFFFFFUL;
f32 constexpr HASH_MAX_F= f32(HASH_MAX);

siz constexpr TOO_BIG= 0x10000000ULL-1;
//no reasonable allocation allowed to excede

using nsec= i64;//nanoseconds are the standard time unit
//for clarity, do not statefully store or pass other time units
//i tried strongly typed implicit scalar basis conversions, but it was against the will of god
using time_t= i64;

constexpr nsec operator""_ns(u64 v){ return v; }
constexpr nsec operator""_us(u64 v){ return v*1'000ll; }
constexpr nsec operator""_ms(u64 v){ return v*1'000'000ll; }
constexpr nsec operator""_s( u64 v){ return v*1'000'000'000ll; }
constexpr time_t usec(nsec t){       return t/1'000ll; }
constexpr time_t msec(nsec t){       return t/1'000'000ll; }
constexpr time_t  sec(nsec t){       return t/1'000'000'000ll; }

}
