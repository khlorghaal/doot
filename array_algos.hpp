#pragma once
#include "arr.hpp"
#include "vector.hpp"

namespace doot{

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

template<typename T>
T average(arr<T> a){
	if(a.size()==0)
		return 0;
	T r= 0;
	for(auto& i: a)
		r+= i;
	r/= a.size();
}

//the additions and removals preformed on a to result in b
//a+adds-rems = b
template<typename T>
inline void difference(arr<T> a, arr<T> b, vector<T> adds, vector<T> rems){
	assert(false);
}

}