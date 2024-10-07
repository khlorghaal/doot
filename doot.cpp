#include "doot.hpp"

//preinclude to have macros work
#include "string.hpp"
#include "bytebuf.hpp"
#include "time.hpp"
#include "math.hpp"
#include "2d.hpp"
#include "file.hpp"

#ifdef DEBUG
	#include "tests.impl.hpp"
#endif

#include "bytebuf.impl.hpp"
#include "timer.impl.hpp"
#include "trans2d.impl.hpp"
#include "thread.impl.hpp"

extc{ void abort(); }

namespace doot{

void _warn(cstr s, cstr m){
	cerr(str(s)+m);
}
void _bad(cstr s, cstr m){
	//todo? idk if i like the concept of bad
	cerr(str(s)+m);
}
void _err(cstr s, cstr m){
	cerr(str(s)+m);
	abort();
};

thread_local hash_t frand_seed= 1;


}
