#pragma once
#include "arr.hpp"
#include "array_algos.hpp"

namespace doot{

template<typename T, size_t cap>
struct ringbuffer{
	static constexpr size_t capacity= cap;

	arr<T> arr= alloc<T>(cap);
	size_t p= 0;
	size_t l= 0;
	
	ringbuffer(){
		fill<T>(arr, 0);
	}
	~ringbuffer(){
		if(!!arr)
			free(arr);
	}

	void operator<<(T t){ push(t); }
	void push(T t){
		arr[p++]= t;
		p%=cap;
		if(l++>cap)
			l= cap;
	}
	T& operator[](size_t i){
		return arr[i%cap];
	}
};

}