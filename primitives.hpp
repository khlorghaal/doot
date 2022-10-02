#pragma once

//this is a header so that externs may be refed without including all of doot

namespace doot{

struct voidstruct{};//used for changing maps into sets, etc

using sizt= unsigned long;
using i8=   char;
using i16=  short;
using i32=  int;
using i64=  long;
using ui8=  unsigned  char;
using ui16= unsigned short;
using ui32= unsigned   int;
using ui64= unsigned  long;
using f32=  float;
using f64=  double;
typedef ui8 byte;

#define null 0
//must define otherwise endless type coercion bitching

using hash_t= ui32;
hash_t constexpr HASH_MAX= 0xFFFFFFFFUL;
float constexpr HASH_MAX_F= float(HASH_MAX);

using sec= i64;
using ms=  i64;
using us=  i64;
using ns=  i64;

using index=  ui32;
typedef index idx;
index constexpr NULLIDX= -1u;
index constexpr nullidx= NULLIDX;

using  id=ui32;
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
