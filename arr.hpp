#pragma once
#include "global.hpp"
#include <new>

namespace doot{

struct arr_void;

template<typename T>
size_t size(T& a){
	return a.end()-a.begin();
}

/*a typed interval of memory*/
template<typename T>
struct arr{
	T* base= NULL;
	T* stop= NULL;
	T* begin() const { return base; }
	T* end()   const { return stop; }
	size_t size() const { return stop-base; }

	arr(){}
	arr(T* a, T* b){ base= a; stop= b; }

	T& operator[](size_t i) const{
		assert( i>=0 & i<size() );
		return base[i];
	}

	idx ptr_idx(T* t) const{
		idx idx= t-base;
		assert(idx>=0 & idx<size());
		return idx;
	}

	idx find(T const& e) const{
		idx i=0;
		for(auto& a: *this){
			if(a==e)
				return i;
			i++;
		}
		return NULLIDX;
	}

	void clear(){ base= stop= NULL; }
	bool operator!(){ return !base; }

	explicit operator arr_void();
};
template<typename T>
const arr<T> EMPTY= arr<T>(NULL, NULL);

#define arrayable(b,e) \
T* begin(){ return b; }\
T* end(){ return e; }\
size_t size(){ return end()-begin(); }\
operator doot::arr<T>(){ return doot::arr<T>{begin(),end()}; }

struct arr_void{
	void* base;
	void* stop;
	
	template<typename T>
	arr<T> cast(){ return {(T*)base, (T*)stop}; }
};
//disallow arr<void>
template<> struct arr<void>{ arr()= delete; };


//c's fixed arrays result in ugly, these are somewhat better
//does not like constness
//copymoveable
template<typename T, size_t CAP>
struct fixedarr{
	T base[CAP];
	arrayable(base,base+CAP)//cant inherit arr because arr is pointers

	T& operator[](size_t i){
		assert(i>=0 & i<CAP);
		return base[i];
	}
};


/*a vector of fixed capacity
pronounced dinner

nonconst methods invalidate indices

uses element constructors
*/
template<typename T, size_t CAP>
struct dynarr: fixedarr<T,CAP>{
	size_t stop= 0;//copymove safe
	arrayable(base, base+stop);

	~dynarr(){
		clear();
	}
	void clear(){
		for(T& t : *this)
			t.~T();
		stop= 0;
	}

	T* alloc(){
		if(stop>=CAP)
			return 0;
		return new (base+stop++) T();
	}
	void push(T e){
		*alloc()= e;
	}

	void rem(size_t idx){
		T& e= operator[](idx);
		e.~T();
		assert(stop>0);
		base[idx]= base[--stop];
	}


	T& operator[](size_t i){
		assert(i>=0 & i<stop);
		return base[i];
	}
};






//eschew using these in favor of passing a vector& or stackarr
template<typename T>
inline arr<T> alloc(size_t n){
	arr<T> ret;
	ret.base= (T*)malloc(n*sizeof(T));
	ret.stop= ret.base+n;
	assert(!!ret.base);
	return ret;
}
template<typename T>
inline void free(arr<T>& a){
	if(!!a.base)
		::free(a.base);
	a.clear();
}
inline void free(arr_void& a){
	if(!!a.base)
		::free(a.base);
	a.base= a.stop= NULL;
}

template<typename T>
inline void copy(arr<T>& dst, arr<T>& src){
	if(dst.size()!=src.size())
		throw;
	memcpy(dst.base,src.base, a.size()*SIZT)
}



template<typename T>
arr<T>::operator arr_void(){
	return {(void*)base,(void*)stop};
}

}