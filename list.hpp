#pragma once
#include "arr.hpp"

namespace doot{
/*
all pointers to elements are invalidated upon nonconst method invocation
reallocation ignores move and copy constructors
template checks if object has init function or not, calls upon allocation

subtractions and insertions fuck sorting

TODO heap container that is a generic allocation owner as a superclass of list
*/
tplt struct list: arr<T>, container{
	using arr<T>::base;
	using arr<T>::stop;
	T* cap;

	static cex siz GROW_FACTOR= 4;
	static cex siz CAP_DEFAULT= 16;//8*16=128

	list(siz init_cap);
	list(): list(CAP_DEFAULT){};
	list(list<T>&& b){
		waive(*this, b); }
	list(list<T> cre b):
		list(b.size()){
		addl(b);}
	list<T>& op=(list<T>&& b){
		clear();
		waive(*this, b); reth; }
	list<T>& op=(list<T> cre b){
		clear();
		addl(b); reth; }
	~list();
	
	siz size() cst{     re stop-base;  }
	siz capacity() cst{ re  cap-base;   }
	bool empty() cst{   re base==stop; }

	//sets capacity, copies elements
	void realloc(siz);
	//will only grow and never shrink
	void prealloc(siz c);
	//realloc capacity*grow_factor
	void expand();

	//args forward to ctor
	tples T& add( E&&... e);
	tples T& op+=(E&&... e){ re add(e...); };

	//appends b to this
	//this must be explicitly seperate from add
	//	i dont remember why but im quite sure
	//	something to do with variad disambiguation?
	void addl(arr<T> cre b){
		prealloc(size()+b.size());
		EACH(e,b)
			add(e);}


	//container placement requires forwarding
	//this is evitablent,
	
	//pushes if element is not contained
	void push_nodup(T cre e);
	
	void insert(idx i, T cre e);
	void push_front(T cre e){ insert(0,e); }

	//remove and return last element
	T pop(){ ass(size()>0); return *--stop; }
	//remove and return first element, swaps with end
	T pop_front();
	
	//swaps element with last and shortens
	void sub_idx(idx i);
	//ret true if contained element
	bool sub_eq(T cre e);	

	void sub(arr<idx>);//param must be sorted ascending
	void op-=(arr<idx> a){ sub(a); }

	void clear();
};

tpl<> struct list<void>{
	void* base;
	void* stop;
	void* cap;
	list(void)= default;
	tple list(list<E>& b){
		*this= rcas<list<void>>(b);
	}
	tplt op list<T>&(){
		re     rcas<list<T>   >(*this); }
};
tplt list<void>& vcas(list<T>& a){
		re rcas<list<void>>(a); }

tpl<typn T, auto cond> void filter_sub(list<T>& v){
	list<idx> d;
	EN(i,e,v){
		if(cond(e))
			d+=i;
	}
	v-=d;
};
tplt void list<T>::sub(arr<idx> d){
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
tplt void waive(list<T>& d, list<T>& s){
	if(d.base!=null && d.size()==0)
		warn("list waive destination not empty");
	d.base= s.base;
	d.stop= s.stop;
	d.cap=  s.cap;
	s.base= s.stop= s.cap= 0;
};
tplt void waive(arr<T>& d,list<T>& s){
	d.base= s.base;
	d.stop= s.stop;
	s.base= s.stop= s.cap= 0;
};

tplt list<T>::list(siz init_cap){
	if(init_cap!=0){
		auto a= doot::alloc<T>(init_cap);
		base= a.base;
		stop= base;
		cap= a.stop;
	}
	else
		base= stop= cap= 0;
}

tplt list<T>::~list(){
	ass(!!base);//temporary for detecting freed lists
	if(!!base)//MUST NOT be invoked on uninitialized memory
		clear();
	doot::free(*this);
}

tplt void list<T>::realloc(siz n){
	siz siz= size();
	ass(n>=siz);

	if(!!base)
		doot::realloc<T>(*this, n);//violates namespace, swap cap and stop
	else
		base= doot::alloc<T>(n).base;

	cap= base+n;
	stop= base+siz;
	ass(stop<=cap);
}

tplt void list<T>::prealloc(siz n){
	siz c= capacity();
	if(n<c)
		return;
	c= c*GROW_FACTOR;
	n= n>c?n:c;
	realloc(n);
};
tplt void list<T>::expand(){
	siz c= capacity()*GROW_FACTOR;
	if(c==0)
		c= CAP_DEFAULT;
	realloc(c);
	ass(cap>stop);
}

tplt tples
T& list<T>::add(E&&... e){
	if(stop==cap)
		expand();
	ass(stop<cap);
	re *new(stop++)T(e...);
}


//pushes if element is not contained
tplt void list<T>::push_nodup(T cre e){
	if(find(*this,e)==NULLIDX)
		make(e);
}

tplt void list<T>::insert(idx i, T cre e){
	ass(false);//lol
}
tplt T list<T>::pop_front(){
	ass(size()>0);
	T ret= base[0];
	sub_idx(0);
	return ret;
}

//swaps element with last and shortens
tplt void list<T>::sub_idx(idx i){
	ass(i>=0&i<size());
	base[i].~T();
	base[i]= *--stop;
}
//ret true if contained element
tplt bool list<T>::sub_eq(T cre e){
	siz i= find(*this,e);
	if(i==NULLIDX)
		return false;
	sub_idx(i);
	return true;
}

tplt void list<T>::clear(){
	for(auto& e: *this)
		e.~T();
	stop= base;
}

}