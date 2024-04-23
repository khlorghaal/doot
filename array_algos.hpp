#pragma once
#include "arr.hpp"
#include "vector.hpp"

namespace doot{

template<typename T>
inline void sort_bubble(arr<T> a){
	if(a.size()<=1)
		return;
	bool done= false;
	while(1){
		done= true;
		for(int i=0; i!=a.size()-1; i++){
			if(a[i]>a[i+1]){
				swap(a[i],a[i+1]);
				done= false;
			}
		}
	}
}

tplt T average(arr<T> a){
	if(a.size()==0)
		return 0;
	T r= 0;
	for(auto& i: a)
		r+= i;
	r/= a.size();
}

//the additions and removals preformed on a to result in b
//a+adds-rems = b
tplt void difference(arr<T> a, arr<T> b, vector<T> adds, vector<T> rems){
	ass(false);
	//todo
}

tplt T sum(arr<T> a){
	T r;
	EACH(o,a)
		r+= o;
	retr;
}

//remainder is given to last segment
tplt void div(arr<T> a, siz denom, vector<arr<T>>& b){
	siz n= a.size();
	siz d= denom;
	if(d==0 || n==0)
		re;
	siz rem= n%d;

	if(d>n){

	}
	else{

	}

	RA(i,n){
		b+= arr<T>{
			base + i,
			base + i+d
		};
	}
	retr;
}

tplt void flatten(arr<arr<T>> aa, vector<T>& r){
	r.realloc_greed(aa.size()*8);
	EACH(a,aa)
		r+= a;
}

}