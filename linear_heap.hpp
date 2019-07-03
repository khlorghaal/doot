#pragma once
#include "doot.hpp"
#include "vector.hpp"

//DEPRECATED - the presence of deleted elements makes this much less useful than mapped_heap

namespace doot{

//maintains order such that indices (but not pointers) remain valid
template<typename T>
struct linear_heap{
	vector<T> heap;
	vector<size_t> freed;
	
	T* alloc(){
		if(freed.empty())
			return heap.alloc();
		else
			return heap+(--freed.stop);
	}
	void free(size_t i){
		#ifdef DEBUG
		assert(i>=0 && i<heap.size());
		memset(heap.base+i,0xFE,SIZEOFT);
		#endif

		freed<<i;
	}
	void free(T* t){ free(t-heap.base); }
	T& operator[](size_t i){
		assert(i>=0 && i<heap.size());
		return heap.base[i];
	}
};

}