#pragma once
#include "arr.hpp"
#include "array_algos.hpp"

namespace doot{

template<typename T, size_t cap>
struct ringbuffer{
	T* arr= 0;
	size_t p= 0;

	arrayable( arr, arr+cap )

	static size_t constexpr capacity= cap;

	ringbuffer(){
		arr= alloc<T>(cap).base;
		fill<T>(*this, 0);
	}
	~ringbuffer(){
		if(!!arr)
			::free(arr);
		arr= 0;
	}

	void operator<<(T t){ push(t); }
	void push(T t){
		arr[p++]= t;
		p%=cap;
	}
	T& operator[](size_t i){
		return arr[i%cap];
	}
};

}