#pragma once
#include "arr.hpp"
#include "thread.hpp"

namespace doot{

//consecutive pushes without a pull overwrite eachother
tplt struct buffer{
	mutex mut;
	maybe<T> b={};
	
	void push(T* t){
		mut.lock();
		if(b)
			delete b.t;
		b= {t};
		mut.locknt();
	};
	//copy from buffer
	maybe<T> pull(){
		mut.lock();
		maybe<T> r= b;
		b= {};
		mut.locknt();
		retr;
	};
};

}