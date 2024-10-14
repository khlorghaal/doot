#ifdef PP_A
	#define PP_B
#define

#define PP_A
#ifndef PP_B
	#include "pp.h"
#endif

#define OF6(IOTA6)

#define PP_B
#endif

#ifndef PP_A

//the c preprocessor is turing complete if provided with sufficient symbols
//inspired by cppcomplete@orangeduck

#define VOID 
#define QUOT(S) S VOID
#define EVAL(S) S


#define FOM FOM##N
#define FOM1( F,OP, L0) F(L0)
#define FOM2( F,OP, L0,L1) F(L0) OP F(L1)
#define FOM3( F,OP, L0,L1,L2) F(L0) OP F(L1) OP F(L2) 
#define FOM4( F,OP, L0,L1,L2,L3) F(L0) OP F(L1) OP F(L2) OP F(L3)
#define FOM5( F,OP, L0,L1,L2,L3,L4) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4)
#define FOM6( F,OP, L0,L1,L2,L3,L4,L5) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5)
#define FOM7( F,OP, L0,L1,L2,L3,L4,L5,L6) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6)
#define FOM8( F,OP, L0,L1,L2,L3,L4,L5,L6,L7) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7)
#define FOM9( F,OP, L0,L1,L2,L3,L4,L5,L6,L7,L8) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7) OP F(L8)
#define FOM10(F,OP, L0,L1,L2,L3,L4,L5,L6,L7,L8,L9) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7) OP F(L8) OP F(L9)
#define FOM11(F,OP, L0,L1,L2,L3,L4,L5,L6,L7,L8,L9,L10) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7) OP F(L8) OP F(L9) OP F(L10)
#define FOM12(F,OP, L0,L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7) OP F(L8) OP F(L9) OP F(L10) OP F(L11)
#define FOM13(F,OP, L0,L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11,L12) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7) OP F(L8) OP F(L9) OP F(L10) OP F(L11) OP F(L12) 
#define FOM14(F,OP, L0,L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11,L12,L13) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7) OP F(L8) OP F(L9) OP F(L10) OP F(L11) OP F(L12) OP F(L13)
#define FOM15(F,OP, L0,L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11,L12,L13,L14) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7) OP F(L8) OP F(L9) OP F(L10) OP F(L11) OP F(L12) OP F(L13) OP F(L14)
#define FOM16(F,OP, L0,L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11,L12,L13,L14,L15) F(L0) OP F(L1) OP F(L2) OP F(L3) OP F(L4) OP F(L5) OP F(L6) OP F(L7) OP F(L8) OP F(L9) OP F(L10) OP F(L11) OP F(L12) OP F(L13) OP F(L14) OP F(L15)

#define IOTA(N) IOTA##N
#define IOTA1  0
#define IOTA2  0, 1
#define IOTA3  0, 1, 2,
#define IOTA4  0, 1, 2, 3
#define IOTA5  0, 1, 2, 3, 4
#define IOTA6  0, 1, 2, 3, 4, 5
#define IOTA7  0, 1, 2, 3, 4, 5, 6
#define IOTA8  0, 1, 2, 3, 4, 5, 6, 7
#define IOTA9  0, 1, 2, 3, 4, 5, 6, 7, 8
#define IOTA10 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
#define IOTA11 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
#define IOTA12 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
#define IOTA13 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
#define IOTA14 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
#define IOTA15 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
#define IOTA16 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,

#define TRIANGLE(N) FOM##N( IOTA,VOID,IOTA##N )

#define VARIAD(X)  FOM8(X, VOID, TRIANGLE(8))

#endif