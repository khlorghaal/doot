/* iosbytestream.cpp
 *
 *  Created on: Jun 24, 2015
 *  Author: Khlorghaal
 *	All Rights Reserved
 *	Do not distribute.
 */

#include "bytebuf.hpp"
#include "string.hpp"

bytebuf::bytebuf(uint8* ptr, size_t cap) : start(ptr), pos(0), size(0), cap(cap){}

void bytebuf::clear(){
	pos= size= 0;
}

void bytebuf::flip(){
	size= pos+1;
	pos= 0;
}

//void bytebuf::grow(size_t cap_){
////	if(cap_>cap){
////		cap= cap_*2;
////		uint8_t* old= start;
////		start= new uint8_t[cap];
////		memcpy(start, old, size);
////		if(cap!=size)
////			memset(start+size, 0, cap-size);
////		delete[] old;
////	}
//}

void bytebuf::write(const void* ptr, size_t len){
	size_t npos= pos+len;
	if(npos>size)
		size= npos+1;
	memcpy(start+pos, ptr, len);
	pos= npos;
}

void bytebuf::read(void* ptr, size_t len){
	size_t npos= pos+len;
	memcpy(ptr, start+pos, len);
	pos= npos;
}



void vecbuf::write(const void* ptr, size_t len){
	size_t npos= pos+len;
	if(npos>v.size())
		v.realloc(npos+1);

	memcpy(&v[0]+pos, ptr, len);
	pos= npos;
}
void vecbuf::read(void* ptr, size_t len){
	size_t npos= pos+len;
	memcpy(ptr, &v[0]+pos, len);
	pos= npos;
}
