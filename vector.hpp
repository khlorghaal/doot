#pragma once
#include "arr.hpp"

namespace doot{
/*
all pointers to elements are invalidated upon nonconst method invocation
reallocation ignores move and copy constructors
template checks if object has init function or not, calls upon allocation
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
	void operator=(vector<T>&& m){
		base= m.base;
		stop= m.stop;
		cap=  m.cap;
		m.release();
	}
	~vector();
	void copy(vector const& that);//that into this
	
	//release data from this's ownership by nulling this
	arr<T> release();
	
	size_t size() const{     return stop-base;  }
	size_t capacity() const{ return cap-base;   }
	bool empty() const{      return base==stop; }

	//sets capacity, copies elements
	void realloc(size_t);
	//will only grow and never shrink
	void realloc_greed(size_t c){ if(c>capacity()) realloc(c); };
	//realloc capacity*grow_factor
	void expand();

	T& alloc();//no ctor
	T& push(T const& e);//copy assign
	T& push(T&&);//move assign
	T& push(){ return push(T{}); };

	template<typename E>
	vector<T>& operator<<(E e){ push(e); return *this; }
	void push(vector<T>& that){	for(auto& e:that) push(e); }
	
	//pushes if element is not contained
	void push_nodup(T const& e);
	
	void insert(size_t i, T const& e);
	void push_front(T const& e){ insert(0,e); }

	//remove and return last element
	T pop(){ assert(size()>0); return *--stop; }
	//remove and return first element, swaps with end
	T pop_front();

	//swaps element with last and shortens
	void remove_idx(size_t i);
	//ret true if contained element
	bool remove_eq(T const& e);	
	void clear();
};

template<typename T>
void free(vector<T>& v){
	v.~vector<T>();
}

template<typename T>
vector<T>::vector(size_t init_cap){
	if(init_cap!=0){
		base= doot::alloc<T>(init_cap*SIZT).base;
		assert(!!base);
	}
	else
		base= 0;

	stop= base;
	cap= base+init_cap;
}

template<typename T>
void vector<T>::copy(vector const& that){
	for(auto& e: *this)
		this->push(e);
}

template<typename T>
vector<T>::~vector(){
	clear();
	if(!!base)
		doot::free(base);
	base= stop= cap= 0;
}

//release data from this's ownership by nulling this
template<typename T>
arr<T> vector<T>::release(){
	arr<T> ret= {base,stop};
	base= stop= cap= 0;
	return ret;
}

template<typename T>
void vector<T>::realloc(size_t l){
	if(l==0){
		this->~vector<T>();
		return;
	}

	size_t siz= size();
	assert(l<TOO_BIG);
	if(!!base)
		base= doot::realloc<T>(base, l);
	else
		base= doot::alloc<T>(l).base;
	assert(!!base);

	cap= base+l;
	stop= base+siz;
	if(cap<stop)
		stop=cap;
}
template<typename T>
void vector<T>::expand(){
	size_t c= capacity()*GROW_FACTOR;
	if(c==0)
		c= CAP_DEFAULT;
	realloc(c);
	assert(cap>stop);
}

template<typename T>
T& vector<T>::alloc(){
	if(stop==cap)
		expand();
	assert(stop<cap);
	return *stop++;
}
template<typename T>
T& vector<T>::push(T&& t){
	T& a= alloc();
	a= static_cast<T&&>(t);
	//xval params are implicitly converted to rval; yes that is unsane
	return a;
}
template<typename T>
T& vector<T>::push(T const& t){
	T& a= alloc();
	a= t;
	return a;
}


//pushes if element is not contained
template<typename T>
void vector<T>::push_nodup(T const& e){
	if(find(*this,e)==NULLIDX)
		push(e);
}

template<typename T>
void vector<T>::insert(size_t i, T const& e){
	assert(false);//lol
}
template<typename T>
T vector<T>::pop_front(){
	assert(size()>0);
	T ret= base[0];
	remove_idx(0);
	return ret;
}

//swaps element with last and shortens
template<typename T>
void vector<T>::remove_idx(size_t i){
	assert(i>=0&i<size());
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