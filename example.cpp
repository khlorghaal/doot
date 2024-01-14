#! clang++ -shared doot.cpp wrap_os.cpp wrap_std.cpp -o doot.so && \
   clang++ example.cpp -L. -ldoot -o example

#include "doot.cpp"
using namespace doot;

/*dootmain handles
	terminal window allocation
	warp initialization
disable with #define DOOTNOMAIN before inclusion*/
void dootmain(){
	arr<str> speaks= {"doot","honk","beep","boop"};
	str doots= WARP(speak,table){//a warp is a parallel-for
		cout(speak);//console io is thread safe
		ret  speak;
	};
	//warp automatically gathers results using `sum`
	cout(doots);

	//see tests.impl.cpp for thorough examples
};