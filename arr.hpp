#pragma once
#include "doot.hpp"

namespace doot{

/*a typed interval of memory
arrays are not a container
 they do not have ownership of their data
 ownership is often implemented by subclasses
*/
template<typename T>
struct arr{
	T* base= 0;
	T* stop= 0;
	T* begin() const { return base; }
	T* end()   const { return stop; }
	sizt size() const { return stop-base; }

	arr(){}
	arr(T* a, T* b){ base= a; stop= b; }

	T& operator[](idx i) const{
		ass( i>=0 & i<size() );
		return base[i];
	}

	idx ptr_idx(T* t) const{
		idx idx= t-base;
		if(idx<0 || idx>=size() || !base )
			return nullidx;
		return idx;
	}

	idx find(T const& e) const{
		idx i=0;
		for(auto& a: *this){
			if(a==e)
				return i;
			i++;
		}
		return nullidx;
	}

	void clear(){ base= stop= null; }
	bool operator!(){ return !base; }
};
template<typename T>
const arr<T> EMPTY= arr<T>(0,0);

//useful when
// something doesnt want to inherit arr's pointers
// something has multiple arrays and only wants to iterate one
#define arrayable(b,e) \
T* begin(){ return b; }\
T* end(){ return e; }\
sizt size(){ return end()-begin(); }\
operator arr<T>(){ return arr<T>{begin(),end()}; }

//c's fixed arrays result in ugly, these are somewhat better
//does not like constness
//copymoveable
template<typename T, sizt CAP>
struct fixedarr{
	T base[CAP];
	arrayable(base,base+CAP)

	T& operator[](idx i){
		ass(i>=0 & i<CAP);
		return base[i];
	}
};


/*a vector of fixed capacity
pronounced dinner

nonconst methods invalidate indices

uses element constructors
*/
template<typename T, sizt CAP>
struct dynarr: fixedarr<T,CAP>{
	using fixedarr<T,CAP>::base;
	sizt stop= 0;//copymove safe
	arrayable(base, base+stop);

	//no copy/assign since shallow/deep ambiguous

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

	void rem(idx idx){
		T& e= operator[](idx);
		e.~T();
		ass(stop>0);
		base[idx]= base[--stop];
	}


	T& operator[](idx i){
		ass(i>=0 & i<stop);
		return base[i];
	}
};



//probably the dootest 3 lines of doot
//may only be used by templates below
void* _malloc(sizt);
void  _free(void*);
void* _realloc(void*,sizt);

//eschew these for containers
//containers use these
tplt inl arr<T> alloc(sizt n){
	arr<T> ret;
	if(n>TOO_BIG)
		err("alloc TOO_BIG");
	ret.base= (T*)_malloc(n*TSIZ);
	ret.stop= ret.base+n;
	if(!ret.base)
		err("OOM");
	retret;
}
tplt inl void realloc(arr<T>& r, sizt n){
	if(!r){
		warn("realloc on unalloced array");
		r= alloc<T>(n);
		re;
	}
	ass(!!r);
	sizt s= n*TSIZ;
	if(s>TOO_BIG)
		err("alloc TOO_BIG");
	r.base= (T*)_realloc(r.base,s);
	r.stop= r.base+n;
	if(!r.base)
		err("OOM");
}
template<typename T>
inline void free(arr<T>& a){
	if(!a.base)
		return;
	_free(a.base);
	a= EMPTY<T>;
}


tplt struct arr_raii: arr<T>{
	arr_raii(siz s){
		arr<T> a= alloc<T>(s);
		arr<T>::base= a.base;
		arr<T>::stop= a.stop;
	}
	~arr_raii(){ free<T>(*this); }
};


template<typename T>
inline void copy(arr<T> dst, arr<T> src){
	if(dst.size()!=src.size())
		err("array copy size mismatch");
	_memcpy(dst.base,src.base, src.size()*TSIZ);
}

template<typename T>
inline void fill(arr<T> a, T val){
	for(auto& e : a)
		e= val;
}
tplt inline void zero(arr<T> a){
	_memclr(a.base, a.size()*TSIZ);
}

template<typename T>
inline idx find(arr<T> a, T val){
	idx i= 0;
	for(auto& e : a){
		if(e==val)
			return i;
		i++;
	}
	return nullidx;
}


tplt bool operator==(arr<T> a, arr<T>b){
	if(a.size()!=b.size())
		return false;
	if(a.base==b.base && a.stop==b.stop)
		return true;

	ZIP(x,y,a,b)
		if(unlikely(x!=y))
			return false;
	return true;
}

}