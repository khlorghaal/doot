#pragma once
#include "vector.hpp"

/*
returns an unused index
keeps a list of freed indices which it recycles first,
if none are free it makes a new one

popped indices will have closely adjacent values,
making them suited for associative arrays
*/
namespace doot{

struct index_recycler{
	vector<idx> freed;
	idx next= 0;

	idx pop(){
		if(freed.empty())
			return next++;
		else
			return *(--freed.stop);
	}
	inline idx operator()(){ return pop(); }

	void free(idx i){
		ass(i<next);
		freed.make(i);
	}
};

}