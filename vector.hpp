#pragma once
#include "arr.hpp"

namespace doot{
/*
all pointers to elements are invalidated upon nonconst method invocation
reallocation ignores move and copy constructors
template checks if object has init function or not, calls upon allocation

TODO heap container that is a generic allocation owner as a superclass of vector
*/
template<typename T>
struct vector: arr<T>, container{
	using arr<T>::base;
	using arr<T>::stop;
	T* cap;

	static constexpr size_t GROW_FACTOR= 4;
	static constexpr size_t CAP_DEFAULT= 16;//8*16=128

	vector(size_t init_cap);
	vector(): vector(CAP_DEFAULT){};
	vector(vector&& b){ waive(*this, b);}
	~vector();
	
	size_t size() const{     return stop-base;  }
	size_t capacity() const{ return cap-base;   }
	bool empty() const{      return base==stop; }

	//sets capacity, copies elements
	void realloc(size_t);
	//will only grow and never shrink
	void realloc_greed(size_t c){ if(c>capacity()) realloc(c); };
	//realloc capacity*grow_factor
	void expand();

	//args forward to ctor
	template<typename... E>
	T& make(E const&... e);

	//appends b to this
	void make(arr<T> const& b){
		for(auto const& e: b)
			make<T const&>(e);
	}
	void make(vector<T> const& b){
		make((arr<T> const&)b);
	}
	
	//pushes if element is not contained
	void push_nodup(T const& e);
	
	void insert(size_t i, T const& e);
	void push_front(T const& e){ insert(0,e); }

	//remove and return last element
	T pop(){ ass(size()>0); return *--stop; }
	//remove and return first element, swaps with end
	T pop_front();
	
	//swaps element with last and shortens
	void remove_idx(size_t i);
	//ret true if contained element
	bool remove_eq(T const& e);	
	void clear();
};

//s relinquishes its allocation, bestowing it onto d
//d MUST NOT be initialized, as it will not be destructed
tplt void waive(vector<T>& d, vector<T>& s){
	d.base= s.base;
	d.stop= s.stop;
	d.cap=  s.cap;
	s.base= s.stop= s.cap= 0;
};
tplt void waive(arr<T>& d,vector<T>& s){
	d.base= s.base;
	d.stop= s.stop;
	s.base= s.stop= s.cap= 0;
};

template<typename T>
vector<T>::vector(size_t init_cap){
	if(init_cap!=0){
		auto a= doot::alloc<T>(init_cap);
		base= a.base;
		stop= base;
		cap= a.stop;
	}
	else
		base= stop= cap= 0;
}

template<typename T>
vector<T>::~vector(){
	ass(!!base);//temporary for detecting freed vectors
	if(!!base)//MUST NOT be invoked on uninitialized memory
		clear();
	doot::free(*this);
}

template<typename T>
void vector<T>::realloc(size_t l){
	size_t siz= size();
	ass(l>=siz);

	if(!!base)
		base= doot::realloc<T>(base, l);
	else
		base= doot::alloc<T>(l).base;

	cap= base+l;
	stop= base+siz;
	ass(stop<=cap);
}
template<typename T>
void vector<T>::expand(){
	size_t c= capacity()*GROW_FACTOR;
	if(c==0)
		c= CAP_DEFAULT;
	realloc(c);
	ass(cap>stop);
}

template<typename T>
template<typename... E>
T& vector<T>::make(E const&... args){
	if(stop==cap)
		expand();
	ass(stop<cap);
	return *new(stop++)T((E const&...)args...);
}


//pushes if element is not contained
template<typename T>
void vector<T>::push_nodup(T const& e){
	if(find(*this,e)==NULLIDX)
		make(e);
}

template<typename T>
void vector<T>::insert(size_t i, T const& e){
	ass(false);//lol
}
template<typename T>
T vector<T>::pop_front(){
	ass(size()>0);
	T ret= base[0];
	remove_idx(0);
	return ret;
}

//swaps element with last and shortens
template<typename T>
void vector<T>::remove_idx(size_t i){
	ass(i>=0&i<size());
	base[i].~T();
	base[i]= *--stop;
}
//ret true if contained element
template<typename T>
bool vector<T>::remove_eq(T const& e){
	size_t i= find(e);
	if(i==NULLIDX)
		return false;
	remove_idx(i);
	return true;
}

template<typename T>
void vector<T>::clear(){
	for(auto& e: *this)
		e.~T();
	stop= base;
}

}