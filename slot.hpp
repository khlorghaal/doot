#pragma once
#include "arr.hpp"
#include <mutex>
using std::mutex;

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
template<typename T>
void slot_push(arr<T>& buf, arr<T>& neu){
	free(buf);//overwrite previous if present
	buf= neu;
	neu.clear();//acquire ownership
}
template<typename T>
arr<T> slot_pull(arr<T>& buf){
	arr<T> ret= buf;
	buf.clear();//release ownership
	return ret;//caller becomes owner
}

/*
thread A:
	allocate -> produce -> push
thread B:
	pull -> consume -> free
*/
template<typename T>
void slot_push(mutex& mut, arr<T>& buf, arr<T>& neu){
	mut.lock();
	slot_push(buf, neu);
	mut.unlock();
}
template<typename T>
arr<T> slot_pull(mutex& mut, arr<T>& buf){
	mut.lock();
	arr<T> ret= slot_pull(buf);
	mut.unlock();
	return ret;
}
template<typename T>
struct slot_mut{
	mutex* mut;
	arr<T> buf= ARR_EMPTY;

	slot_mut(mutex* mut_){ mut= mut_; }

	void push(arr<T> neu){ slot_push_mut(*mut, buf, neu); }
	arr<T> pull(){  return slot_pull_mut(*mut, buf); }
};