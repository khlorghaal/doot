#pragma once
#include "arr.hpp"
#include "vector.hpp"

#define NO_MULTITHREAD true

namespace tasker{


template<typename T, void(*task)(arr<T>) >
inline void invoke(arr<T> jobs);


extern int poolsize;//excludes invoking working thread
extern volatile bool active;

void init();

//can be used for hopping update in attition to polythreading
//number of segments given from size() of ret
template<typename T>
void segment(arr<T> const jobs, arr<arr<T>>& ret);

}
