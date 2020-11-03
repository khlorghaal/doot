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

void* __malloc(  size_t s){	     	return ::malloc( s); };
void  __free(   void* p){				   ::free(   p); };
void* __realloc(void* p, size_t s){	return ::realloc(p,s); };

void __memcpy(void* dst, void* src, size_t len){
	ass(!!dst&!!src);
	::memcpy(dst,src,len);
}

void bad(char const* m){
	cout(m);
}
void err(char const* m){
	cout(m);
	throw;
};
void err(){err("");}
void bad(){bad("");}

thread_local hash_t frand_seed= 1;

}