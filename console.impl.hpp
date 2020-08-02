#include "doot.hpp"
#include <iostream>
#include <mutex>

namespace doot{

static std::mutex mut;

console cout;
console cerr;

console& console::op()(str const& x){
	mut.lock();
	std::cout<<x.cstr()<<std::endl;
	mut.unlock();
	retthis;
};

}