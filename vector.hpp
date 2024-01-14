#pragma once
#include "arr.hpp"

namespace doot{
/*
all pointers to elements are invalidated upon nonconst method invocation
reallocation ignores move and copy constructors
template checks if object has init function or not, calls upon allocation

subtractions and insertions fuck sorting

TODO heap container that is a generic allocation owner as a superclass of vector
*/
tplt struct vector: arr<T>, container{
	using arr<T>::base;
	using arr<T>::stop;
	T* cap;

	static cex sizt GROW_FACTOR= 4;
	static cex sizt CAP_DEFAULT= 16;//8*16=128

	vector(sizt init_cap);
	vector(): vector(CAP_DEFAULT){};
	vector(vector&& b){ waive(*this, b);}
	~vector();
	
	sizt size() const{     return stop-base;  }
	sizt capacity() const{ return cap-base;   }
	bool empty() const{    return base==stop; }

	//sets capacity, copies elements
	void realloc(sizt);
	//will only grow and never shrink
	void realloc_greed(sizt c);
	//realloc capacity*grow_factor
	void expand();

	//args forward to ctor
	tpl<typn... E>
	T& add(E const&... e);
	tpl<typn... E>
	T& operator+=(E const&... e){ return add(e...); };

	//appends b to this
	void addv(vector<T> cst& b){
		addv((arr<T>&)b);
	}
	void addv(arr<T> cst& b){
		realloc_greed(size()+b.size());
		for(T& e:b)
			add<T>(e);
	}
	
	//pushes if element is not contained
	void push_nodup(T const& e);
	
	void insert(idx i, T const& e);
	void push_front(T const& e){ insert(0,e); }

	//remove and return last element
	T pop(){ ass(size()>0); return *--stop; }
	//remove and return first element, swaps with end
	T pop_front();
	
	//swaps element with last and shortens
	void sub_idx(idx i);
	//ret true if contained element
	bool sub_eq(T const& e);	

	void sub(arr<idx>);//param must be sorted ascending
	void op-=(arr<idx> a){ sub(a); }

	void clear();
};

tpl<typn T, auto cond> void filter_sub(vector<T>& v){
	vector<idx> d;
	EN(i,e,v){
		if(cond(e))
			d+=i;
	}
	v-=d;
};
tplt void vector<T>::sub(arr<idx> d){
	u64 pi= -1;
	EACHD(e,d){//descending, remove from end
		#ifdef DEBUG
			//ordering is crucial
			ass(e<pi);
			pi= e;
		#endif
		sub_idx(e);
	}
};

//s relinquishes its allocation, stowing d
//d MUST NOT be initialized, as it will not be destructed
tplt void waive(vector<T>& d, vector<T>& s){
	if(d.base!=null && d.size()==0)
		warn("vector waive destination not empty");
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

tplt vector<T>::vector(sizt init_cap){
	if(init_cap!=0){
		auto a= doot::alloc<T>(init_cap);
		base= a.base;
		stop= base;
		cap= a.stop;
	}
	else
		base= stop= cap= 0;
}

tplt vector<T>::~vector(){
	ass(!!base);//temporary for detecting freed vectors
	if(!!base)//MUST NOT be invoked on uninitialized memory
		clear();
	doot::free(*this);
}

tplt void vector<T>::realloc(sizt l){
	sizt siz= size();
	ass(l>=siz);

	if(!!base)
		base= doot::realloc<T>(base, l);
	else
		base= doot::alloc<T>(l).base;

	cap= base+l;
	stop= base+siz;
	ass(stop<=cap);
}

tplt void vector<T>::realloc_greed(sizt c){
	if(c<capacity())
		return;
	c*= GROW_FACTOR;
	T* n= doot::alloc<T>(c).base;
	sizt s= size();
	copy<T>({n,n+s},*this);
	_free(base);
	base= n;
	stop= n+s;
	cap = n+c;
	
	ass(stop<=cap);
};
tplt void vector<T>::expand(){
	sizt c= capacity()*GROW_FACTOR;
	if(c==0)
		c= CAP_DEFAULT;
	realloc(c);
	ass(cap>stop);
}

tplt
tpl<typn... E>
T& vector<T>::add(E const&... e){
	if(stop==cap)
		expand();
	ass(stop<cap);
	return *new(stop++)T(e...);
}


//pushes if element is not contained
tplt void vector<T>::push_nodup(T const& e){
	if(find(*this,e)==NULLIDX)
		make(e);
}

tplt void vector<T>::insert(idx i, T const& e){
	ass(false);//lol
}
tplt T vector<T>::pop_front(){
	ass(size()>0);
	T ret= base[0];
	sub_idx(0);
	return ret;
}

//swaps element with last and shortens
tplt void vector<T>::sub_idx(idx i){
	ass(i>=0&i<size());
	base[i].~T();
	base[i]= *--stop;
}
//ret true if contained element
tplt bool vector<T>::sub_eq(T const& e){
	sizt i= find(*this,e);
	if(i==NULLIDX)
		return false;
	sub_idx(i);
	return true;
}

tplt void vector<T>::clear(){
	for(auto& e: *this)
		e.~T();
	stop= base;
}

}