#pragma once
#include "arr.hpp"
#include "list.hpp"
#include "math.hpp"

namespace doot{

tplt void sort_bubble(arr<T> a){
	if(a.size()<=1)
		re;
	bool done= false;
	while(1){
		done= true;
		ra(i,a.size()-1){
			if(a[i]>a[i+1]){
				swap(a[i],a[i+1]);
				done= false;
			}
		}
	}
}

tplt T average(arr<T> a){
	if(a.size()==0)
		re 0;
	T r= 0;
	each(e,a)
		r+= e;
	r/= a.size();
	retr;
}

//the additions and removals preformed on a to result in b
//a+adds-rems = b
tplt void difference(arr<T> a, arr<T> b, list<T> adds, list<T> rems){
	ass(false);
	//todo
}

tplt T sum(arr<T> a){
	T r=0;
	each(o,a)
		r+= o;
	retr;
}

//remainder is given to last segment
tplt void div(arr<T> a, siz divisor, list<arr<T>>& b){
	//todo testcases

	T* base= a.base;
	siz n= a.size();//elements
	siz d= divisor;//segment size
	if(d==0 || n==0)
		re;
	siz rem= n%d;//distributed heuristically


	//distribute
	if(n>d){
		//normal case
		// denomination more than elements
		siz span= n/d;//per thread, not stride
		ass(rem>=0);
	
		ra(i,d){
			b.add(arr<T>{
				base+ i*span,
				base+ i*span+d,
			});
		}
		//give remainder to last thread
		//worstcase: n= d*2-1
		//	todo the remainder should be dsitributed evenly
		(b.stop-1)->stop+= rem;//b:=stop
	
	}else{
		//output size will be equal to elements, less than divisor
		ra(i,n){
			b.add(arr<T>{
				base+ i  ,
				base+ i+1
			});
		}
	}

}

#define FLAT(aa,r) \
	r.prealloc(aa.size()*4);\
	each(a,aa) r+= a;

}