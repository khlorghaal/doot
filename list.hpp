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
	T* cap= null;

	sex siz GROW_FACTOR= 4;
	sex siz CAP_DEFAULT= 16;//4*16=64 cache line size

	list(siz init_cap);
	list(): list(CAP_DEFAULT){};
	list(list<T>&& b){
		base=0;
		acquire(*this, b); }
	list(list<T> cre b):
		list(b.size()){
		addl(b);}
	list<T>& op=(list<T>&& b){
		clear();
		acquire(*this, b); reth; }
	list<T>& op=(list<T> cre b){
		clear();
		addl(b); reth; }
	~list();
	
	siz size() cst{     re stop-base;  }
	siz capacity() cst{ re  cap-base;   }
	bool empty() cst{   re base==stop; }

	//sets capacity, copies elements
	//does not round up, stop==cap
	void realloc(siz);
	//will only grow and never shrink
	void prealloc(siz c);
	//realloc capacity*grow_factor
	void expand();

	T* _alloc();
	tples T&  add(E&&... e){//forward to ctor
		re *new(_alloc())T( (E&&)e... );}
	tples T& init(E&&... e){//forward to initer
		re *new(_alloc())T({(E&&)e...});}
	tples T& op+=(E&&... e){ re add((E&&)e...); };

	//appends b to this
	//this must be explicitly seperate from add
	//	dont remember why but quite sure something to do with variad disambiguation?
	tple void addl(arr<E> cre b){
		prealloc(size()+b.size());
		each(e,b)
			add((E cst)e);}
			//xval handling is fucked, dont


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
	en(i,e,v){
		if(cond(e))
			d+=i;
	}
	v-=d;
};
tplt void list<T>::sub(arr<idx> d){
	u64 pi= -1;
	eachd(e,d){//descending, remove from end
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
tplt void acquire(list<T>& d, list<T>& s){
	if(!!d.base)
		warn("list::acquire destination not empty");
	d.base= s.base;
	d.stop= s.stop;
	d.cap=  s.cap;
	s.base= s.stop= s.cap= 0;//uam safety
};
tplt void acquire(arr<T>& d,list<T>& s){
	d.base= s.base;
	d.stop= s.stop;
	s.base= s.stop= s.cap= 0;
};

tplt list<T>::list(siz init_cap){
	if(init_cap!=0){
		auto a= doot::alloc<T>(init_cap);
		ass(!base & !stop & !cap);
		base= a.base;
		stop=   base;
		 cap= a.stop;}
	else
		base= stop= cap= 0;
}

tplt list<T>::~list(){
	//ass(!!base);//temporary for detecting freed lists
	if(!!base)//must not be invoked on uninitialized memory
		clear();
	doot::free(*this);
	cap= null;
}

tplt void list<T>::realloc(siz n){
	siz siz= size();
	if(n<siz){//trim end
		ra2(i,n,siz)
			base[i].~T();
		stop= base+n;}

	if(!!base)
		doot::realloc<T>(*this, n);//violates names, swap cap and stop
	else
		base= doot::alloc<T>(n).base;

	stop= base+siz;
	 cap= base+n;
	ass(size()<=capacity());
}

tplt void list<T>::prealloc(siz n){
	siz c= capacity();
	if(n<=c)
		re;
	realloc(n);
};
tplt void list<T>::expand(){
	siz c= capacity()*GROW_FACTOR;
	if(c<=CAP_DEFAULT)
		c= CAP_DEFAULT;
	realloc(c);
	ass(cap>stop);
}


tplt T* list<T>::_alloc(){
	if(stop==cap)
		expand();
	ass(stop<cap);
	re stop++;
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
	re ret;
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
		re false;
	sub_idx(i);
	re true;
}

tplt void list<T>::clear(){
	each(e,*this)
		e.~T();
	stop= base;
}

}