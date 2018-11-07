#pragma once

namespace doot{

struct voidstruct{};
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

typedef uint32 index;
typedef index idx;
constexpr index NULLIDX= -1ul;

typedef uint32 id;
typedef id eid;//entity ID
typedef id CID;//component ID
id constexpr NULLID= -1ul;

size_t constexpr TOO_BIG= 0x10000000ULL-1;
//no reasonable allocation allowed to excede

}
