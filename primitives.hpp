#pragma once

namespace doot{

struct voidstruct{};//used for changing maps into sets, etc

typedef char      int8;
typedef short     int16;
typedef int       int32;
typedef long long int64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef int8 byte;
typedef uint8 ubyte;

typedef int8    i8;
typedef int16   i16;
typedef int32   i32;
typedef int64   i64;
typedef uint8   ui8;
typedef uint16  ui16;
typedef uint32  ui32;
typedef uint64  ui64;
typedef float  f32;
typedef double f64;

byte constexpr NULL= 0;
byte constexpr null= 0;

typedef uint32 hash_t;
hash_t constexpr HASH_MAX= 0xFFFFFFFFUL;

typedef int64 sec;
typedef int64 ms;
typedef int64 us;

typedef uint32 index;
typedef index idx;
index constexpr NULLIDX= -1ul;
index constexpr nullidx= NULLIDX;

typedef uint32 id;
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

size_t constexpr TOO_BIG= 0x10000000ULL-1;
//no reasonable allocation allowed to excede

}
