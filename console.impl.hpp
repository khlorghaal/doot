#include "doot.hpp"
#include <iostream>
#include <mutex>

namespace doot{

static std::mutex mut;

charstream cnsl;

charstream& operator<<(charstream& s, string const& x){
	mut.lock();
	std::cout<<x.cstr();
	mut.unlock();
	return s;
};
charstream& operator<<(charstream& s, endl_t const&){
	mut.lock();
	std::cout<<std::endl;
	mut.unlock();
	return s;
};

}