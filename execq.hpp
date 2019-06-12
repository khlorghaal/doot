#pragma once
//deprecated, use tasker instead

#include "vector.hpp"
#include <mutex>
using std::mutex;

struct exec{
	virtual ~exec()= default;
	virtual void invoke()=0;
};
/*template<typename T, typename... Args>
static struct newer: exec{
Args args;
newer(Args args) args(args){};
void invoke(){ new T(args); } override;
};*/
template<typename T>
struct deleter: exec{
	T* o;
	deleter(T* o): o(o){};
	void invoke() override { delete o; };
};

struct execq{
	mutex mut;
	vector<exec*> q;
	void push(exec* e);
	void flush();
};
