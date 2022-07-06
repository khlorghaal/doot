#pragma once
#include "arr.hpp"
#include "array_algos.hpp"

namespace doot{

template<typename T, sizt cap>
struct ringbuffer{
	static constexpr sizt capacity= cap;

	arr<T> arr= alloc<T>(cap);
	sizt p= 0;
	sizt l= 0;
	
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
	T& operator[](sizt i){
		return arr[i%cap];
	}
};

}