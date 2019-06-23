#pragma once
#include "vector.hpp"

//eschew this in favor of passing vector& outparams

namespace doot{

extern thread_local idx thread_gatherer_idx;
void gather_add_thread();//must be called after forking
void gather_sub_thread();//must be called before joining

template<typename T>
struct gather{
	//thread unsafe
	vector<vector<T>> arrays;

	//adds to the thread_local accumulator
	void push(T t){
		arrays[thread_gatherer_idx].push(t);
	}
};

}
