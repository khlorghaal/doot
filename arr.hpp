#pragma once
#include "doot.hpp"
#include <new>

namespace doot{

/*a typed interval of memory
arrays are not a container
 they do not have ownership of their data
 ownership is often implemented by subclasses
*/
template<typename T>
struct arr{
	T* base= null;
	T* stop= null;
	T* begin() const { return base; }
	T* end()   const { return stop; }
	size_t size() const { return stop-base; }

	arr(){}
	arr(T* a, T* b){ base= a; stop= b; }

	T& operator[](size_t i) const{
		ass( i>=0 & i<size() );
		return base[i];
	}

	idx ptr_idx(T* t) const{
		idx idx= t-base;
		if(idx<0 || idx>=size() || !base )
			return NULLIDX;
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
};
template<typename T>
const arr<T> EMPTY= arr<T>(NULL, NULL);

//useful when
// something doesnt want to inherit arr's pointers
// something has multiple arrays and only wants to iterate one
#define arrayable(b,e) \
T* begin(){ return b; }\
T* end(){ return e; }\
size_t size(){ return end()-begin(); }\
operator arr<T>(){ return arr<T>{begin(),end()}; }


//c's fixed arrays result in ugly, these are somewhat better
//does not like constness
//copymoveable
template<typename T, size_t CAP>
struct fixedarr{
	T base[CAP];
	arrayable(base,base+CAP)

	T& operator[](size_t i){
		ass(i>=0 & i<CAP);
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
	using fixedarr<T,CAP>::base;
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

	template<typename E>
	bool make(E e){
		if(stop>=CAP)
			return false;
		T* a= base+stop++;
		*a= e;
		return true;
	}
	void make(T e){ make<T>(e); }

	void rem(size_t idx){
		T& e= operator[](idx);
		e.~T();
		ass(stop>0);
		base[idx]= base[--stop];
	}


	T& operator[](size_t i){
		ass(i>=0 & i<stop);
		return base[i];
	}
};



//probably the dootest 3 lines of doot
//may only be used by templates below
void* __malloc(size_t);
void  __free(void*);
void* __realloc(void*,size_t);

//eschew these for containers
//containers use these
template<typename T>
inline arr<T> alloc(size_t n){
	arr<T> ret;
	if(n>TOO_BIG)
		err("alloc TOO_BIG");
	ret.base= (T*)doot::__malloc(n*TSIZ);
	ret.stop= ret.base+n;
	if(!ret.base)
		err("OOM");
	return ret;
}
template<typename T>
inline T* realloc(T* p, size_t s){
	ass(!!p);
	if(s>TOO_BIG)
		err("alloc TOO_BIG");
	auto res= (T*)doot::__realloc(p,s*TSIZ);
	if(!res)
		err("OOM");
	return res;
}
template<typename T>
inline void free(arr<T>& a){
	if(!a.base)
		return;
	doot::__free(a.base);
	a= EMPTY<T>;
}



template<typename T>
inline void copy(arr<T>& dst, arr<T>& src){
	if(dst.size()!=src.size())
		err("array copy size mismatch");
	_memcpy(dst.base,src.base, src.size()*TSIZ);
}

template<typename T>
inline void fill(arr<T> a, T val){
	for(auto& e : a)
		e= val;
}

template<typename T>
inline idx find(arr<T> a, T val){
	idx i= 0;
	for(auto& e : a){
		if(e==val)
			return i;
		i++;
	}
	return NULLIDX;
}

}