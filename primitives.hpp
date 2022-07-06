#pragma once
#include <stddef.h>
#include <stdint.h>

namespace doot{

struct voidstruct{};//used for changing maps into sets, etc

typedef size_t sizt;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef uint8_t   ui8;
typedef uint16_t  ui16;
typedef uint32_t  ui32;
typedef uint64_t  ui64;
typedef float  f32;
typedef double f64;

ui8 constexpr NULL= 0ui;
ui8 constexpr null= 0ui;

typedef ui32 hash_t;
hash_t constexpr HASH_MAX= 0xFFFFFFFFUL;

typedef i64 sec;
typedef i64 ms;
typedef i64 us;

typedef ui32 index;
typedef index idx;
index constexpr NULLIDX= -1ul;
index constexpr nullidx= NULLIDX;

typedef ui32 id;
typedef id eid;//entity ID
typedef id cid;//component ID, where an entity possess multiple components, cids are unique per entity
id constexpr NULLID= -1ul;
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
