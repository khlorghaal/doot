#include "doot.hpp"
#include "bytebuf.impl.hpp"
#include "console.impl.hpp"
#include "string.impl.hpp"
#include "timer.impl.hpp"
#include "trans2d.impl.hpp"


#include "weave.impl.hpp"
#include "tests.impl.hpp"

#include "file.impl.hpp"
#ifdef LINUX
#include "os_posix.impl.hpp"
#elif _WIN32
#include "os_win.impl.hpp"
#endif

namespace doot{

void* _malloc(size_t s){
	return ::malloc(s);
};
void  _free(void* p){
	::free(p);
};
void* _realloc(void* p, size_t s){
	return ::realloc(p,s);
};

void bad(){
	throw;
}
void error(char const*){
	throw;
};
void error(){
	error("");
}
void assert(bool x) {
	#ifdef DEBUG
	if(!x)
		error("failed assert");
	#else
	;
	#endif
}

thread_local hash_t frand_seed= 1;

}