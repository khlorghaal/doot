#pragma once

//this is a header so that externs may be refed without including all of doot

namespace doot{

struct voidstruct{};//used for changing maps into sets, etc

typedef unsigned long  sizt;
typedef           char i8;
typedef          short i16;
typedef            int i32;
typedef           long i64;
typedef unsigned  char ui8;
typedef unsigned short ui16;
typedef unsigned   int ui32;
typedef unsigned  long ui64;
typedef float    f32;
typedef double   f64;
typedef ui8 byte;

#define null 0
//must define otherwise endless type coercion bitching

typedef ui32 hash_t;
hash_t constexpr HASH_MAX= 0xFFFFFFFFUL;
float constexpr HASH_MAX_F= float(HASH_MAX);

typedef i64 sec;
typedef i64 ms;
typedef i64 us;
typedef i64 ns;

typedef ui32 index;
typedef index idx;
index constexpr NULLIDX= -1u;
index constexpr nullidx= NULLIDX;

typedef ui32 id;
typedef id eid;//entity ID
typedef id cid;//component ID, where an entity possess multiple components, cids are unique per entity
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
