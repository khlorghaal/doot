#pragma once
#include "arr.hpp"
#include <mutex>
using std::mutex;

//consecutive pushes without a pull overwrite eachother
template<typename T>
struct buffer_overflowy{
	mutex mut;
	T buff;
	
	//copy into buffer
	void push(T t){
		mut.lock();
		buff= t;
		mut.unlock();
	};
	//copy from buffer
	T pull(){
		mut.lock();
		T t= buff;
		mut.unlock();
		return t;
	};
};

//i cannot figure out how to get a nice variadic impl of this