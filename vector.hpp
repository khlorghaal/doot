#pragma once
#include "arr.hpp"

namespace doot{
/*
all pointers to elements are invalidated upon nonconst method invocation
does not use construct or destructors
reallocation ignores move and copy constructors
*/
template<typename T>
struct vector: arr<T>, no_copy, no_assign{
	using arr<T>::base;
	using arr<T>::stop;
	T* cap;

	static constexpr size_t GROW_FACTOR= 4;
	static constexpr size_t CAP_DEFAULT= 16;//8*16=128

	vector(size_t init_cap);
	vector(): vector(CAP_DEFAULT){}
	void copy(vector const& that);//that into this
	~vector();
	
	//release data from this's ownership by nulling this
	void release(T*& ret_base, T*& ret_stop);
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

	T* alloc();
	void push(T const& e){ *alloc()= e; }
	vector<T>& operator<<(T const& e){ push(e); return *this; }
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

	operator string();
};

template<typename T>
void free(vector<T>& v){
	v.~vector<T>();
}

template<typename T>
vector<T>::vector(size_t init_cap){
	if(init_cap!=0){
		base= (T*)malloc(init_cap*SIZT);
		assert(!!base);
	}
	else
		base= 0;

	stop= base;
	cap= base+init_cap;
}

template<typename T>
void vector<T>::copy(vector const& that){
	this->clear();
	this->realloc(that.capacity());
	for(T* i=that.base; i!=that.stop; i++)
		this->push(*i);
}

template<typename T>
vector<T>::~vector(){
	clear();
	if(!!base)
		::free(base);
	base= stop= cap= 0;
}

//release data from this's ownership by nulling this
template<typename T>
arr<T> vector<T>::release(){
	arr<T> ret;
	ret.base= base;
	ret.stop= stop;
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
	assert(l*SIZT<0x1000000);
	if(!!base)
		base= (T*)::realloc(base, l*SIZT);
	else
		base= (T*)::malloc(l*SIZT);
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
T* vector<T>::alloc(){
	if(stop==cap)
		expand();
	assert(stop<cap);
	return stop++;
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
	stop= base;
}

template<typename T>
vector<T>::operator string(){
	string ret;
	ret<<"[ "
	for(auto const& i: *this)
		ret<<*i<<", ";
	ret<<" ]";
}
template<>
inline vector<char>::operator string(){
	string ret;
	bool hadnull= base[size()-1]==0;
	size_t s= size();
	if(!hadnull)
		s++;

	ret.cstr= ::alloc<char>(s).base;
	memcpy(ret.cstr, base, s);
	ret.len= s-1;

	if(!hadnull)
		ret.cstr[s-1]= 0;

	return ret;
}

}