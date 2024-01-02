#pragma once

//this is a header so that externs may be refed without including all of doot

namespace doot{

struct voidstruct{};//used for changing maps into sets, etc

using siz_t= unsigned long long;
using sizt = unsigned long long;
using siz  = unsigned long long;
using i8=   char;
using i16=  short;
using i32=  int;
using i64=  long long;
using  u8= unsigned  char;
using u16= unsigned short;
using u32= unsigned   int;
using u64= unsigned  long long;
using f32=  float;
using f64=  double;

#define null 0
//must define otherwise endless type coercion bitching

#define CEX extern "C"

using hash_t= u32;
hash_t constexpr HASH_MAX= 0xFFFFFFFFUL;
float constexpr HASH_MAX_F= float(HASH_MAX);

using sec= i64;
using ms=  i64;
using us=  i64;
using ns=  i64;

using index=  u32;
using idx= index;;
index constexpr NULLIDX= -1u;
index constexpr nullidx= NULLIDX;

using  id=u32;
using eid=id;//entity ID
using cid=id;//component ID, where an entity possess multiple components, cids are unique per entity
//component arrays prefered over subentities to min deref
id constexpr NULLID= -1u;
id constexpr nullid= NULLID;
struct ecid{
	eid e;
	cid i;
	ecid(): e(NULLID),i(NULLID){};
	ecid(eid e_, id i_): e(e_),i(i_){};
	inline bool operator!(){ return e==NULLID || i==NULLID; };
	inline bool operator==(ecid b){ return e==b.e && i==b.i; };
};//entity->element IDs
const ecid NULLECID= {NULLID,NULLID};

sizt constexpr TOO_BIG= 0x10000000ULL-1;
//no reasonable allocation allowed to excede

}
