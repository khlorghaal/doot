#pragma once
#include "doot.hpp"

namespace doot{

/*a typed interval of memory
arrays are not a container
 they do not have ownership of their data
 ownership is often implemented by subclasses
*/
tplt struct arr{
	T* base= 0;
	T* stop= 0;
	T* begin() cst { re base; }
	T* end()   cst { re stop; }
	sizt size() cst { re stop-base; }
	sizt  len() cst { re size(); }

	arr(){}
	arr(T* a, T* b){ base= a; stop= b; }

	T& op[](idx i) cst{
		ass( i>=0 );
		ass( i<size() );
		re base[i];
	}

	idx ptr_idx(T* t) cst{
		idx idx= t-base;
		if(idx<0 || idx>=size() || !base )
			re nullidx;
		re idx;
	}

	idx find(T cre e) cst{
		idx i=0;
		each(a,*this){
			if(a==e)
				re i;
			i++;
		}
		re nullidx;
	}

	void clear(){ base= stop= null; }
	bool op!() cst{ re !base; }

	op arr<void>() cst;
};
#define EMPTY {0,0}

tplt siz len(T cre a){ re a.size(); }

//useful because
// something doesnt want to inherit arr's pointers
// something has multiple arrays and only wants to iterate one
#define arrayable(b,e) \
T* begin ()cst{ re b; }\
T* end   ()cst{ re e; }\
sizt size()cst{ re  e-b ; }\
op arr<T>(){ re {b,e}; }

tpl<> struct arr<void>{
	void* base;
	void* stop;
	//not arrayable, must be casted
	tplt op arr<T>() cst{ re {(T*)base,(T*)stop}; };
};
tplt arr<T>::op arr<void>() cst{ re {(void*)  base,(void*)  stop}; };
tplt arr<T> vcas(arr<void> a){   re {(   T*)a.base,(   T*)a.stop}; };

//c's fixed arrays result in ugly, these are somewhat better
//does not like constness
//copymoveable
tpl<typn T, sizt CAP>
struct fixedarr{
	T base[CAP];
	arrayable(base,(base+CAP))

	T& op[](idx i){
		ass(i>=0 & i<CAP);
		re base[i];
	}
};


/*a list of fixed capacity
pronounced dinner

nonconst methods invalidate indices

uses element constructors
*/
tpl<typn T, sizt CAP>
struct dynarr: fixedarr<T,CAP>{
	using fixedarr<T,CAP>::base;
	sizt stop= 0;//copymove safe
	arrayable(base, (base+stop));

	//no copy/assign since shallow/deep ambiguous

	~dynarr(){
		clear();
	}
	void clear(){
		each(t, op arr<T>())
			t.~T();
		stop= 0;
	}

	tple bool make(E e){
		if(stop>=CAP)
			re false;
		T* a= base+stop++;
		*a= e;
		re true;
	}
	void make(T e){ make<T>(e); }

	void rem(idx idx){
		T& e= op[](idx);
		e.~T();
		ass(stop>0);
		base[idx]= base[--stop];
	}


	T& op[](idx i){
		ass(i>=0 & i<stop);
		re base[i];
	}
};



//probably the dootest 3 lines of doot
//may only be used by templates below
void* _malloc(sizt);
void  _free(void*);
void* _realloc(void*,sizt);

//eschew these for containers
//containers use these
tplt arr<T> alloc(sizt n){
	arr<T> ret;
	if(n>TOO_BIG)
		err("alloc TOO_BIG");
	ret.base= (T*)_malloc(n*TSIZ);
	ret.stop= ret.base+n;
	if(!ret.base)
		err("OOM");
	retret;
}
tplt void realloc(arr<T>& r, sizt n){
	if(!r){
		warn("realloc on unalloced array");
		r= alloc<T>(n);
		re;
	}
	ass(!!r);
	if(n==0)
		warn("0 size realloc");
	sizt s= n*TSIZ;
	if(s>TOO_BIG)
		err("alloc TOO_BIG");
	r.base= (T*)_realloc(r.base,s);
	r.stop= r.base+n;
	if(!r.base && n!=0)//n==0 may null or freeable addr
		err("OOM");
}
tplt void free(arr<T>& a){
	if(!a.base)
		re;
	_free(a.base);
	a= EMPTY;
}


tplt struct arr_raii: arr<T>{
	arr_raii(siz s){
		arr<T> a= alloc<T>(s);
		arr<T>::base= a.base;
		arr<T>::stop= a.stop;
	}
	~arr_raii(){ free<T>(*this); }
};


tplt void copy(arr<T> dst, arr<T> src){
	if(dst.size()!=src.size())
		err("array copy size mismatch");
	_memcpy(dst.base,src.base, src.size()*TSIZ);
}

tplt void fill(arr<T> a, T val){
	each(e,a)
		e= val;}
tplt void fill(arr<maybe<T>> a, T val){
	each(e,a)
		e= {val};}
tplt void zero(arr<T> a){
	_memclr(a.base, a.size()*TSIZ);}

tplt idx find(arr<T> a, T val){
	en(i,e,a)
		if(unlikely(e==val))
			re i;
	re nullidx;
}

tplt bool op==(arr<T> a, arr<T>b){
	if(a.size()!=b.size())
		re false;
	if(a.base==b.base && a.stop==b.stop)
		re true;

	zip(x,y,a,b)
		if(unlikely(x!=y))
			re false;
	re true;
}

}