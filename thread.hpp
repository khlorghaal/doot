#pragma once
#include "string.hpp"

namespace doot{
struct thread{
	static thread_local string name;
	thread(string, void(*)(void*), void*);
};
}