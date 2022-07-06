#pragma once
#include "arr.hpp"
#include "thread.hpp"

namespace doot{

/*
a slot is a buffer which supports an unbalanced ratio of push:pull
overpushing: frees then overwrites the buffer
overpulling: returns the empty set

states:
	balanced
		allocate -> produce -> push -> buf -> pull -> consume -> free
	overpush
		allocate -> produce -> push -> buf freed+replaced
	overpull
		pull -> consume empty set -> empty set cant/neednt be freed
		
*/
tplt struct slot{
	T* buf;
	mutex mut;

	void push(T* neu){
		mut.lock();
		if(buf!=null)
			delete buf;//overwrite previous if present
		buf= neu;
		mut.locknt();
	}
	T* pull(){
		mut.lock();
		T* ret= buf;
		buf= null;//release ownership
		mut.locknt();
		return ret;//caller becomes owner
	}
};

}