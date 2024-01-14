#include "doot.hpp"

//preinclude to have macros work
#include "string.hpp"
#include "bytebuf.hpp"
#include "timer.hpp"
#include "math.hpp"
#include "thread.hpp"
#include "file.hpp"

#ifdef DEBUG
#include "tests.impl.hpp"
#endif

#include "string.impl.hpp"
#include "bytebuf.impl.hpp"
#include "timer.impl.hpp"
#include "trans2d.impl.hpp"
#include "thread.impl.hpp"

namespace doot{

void _warn(char const* s, char const* m){
	cerr(str(s)+m);
}
void _bad(char const* s, char const* m){
	//todo? idk if i like the concept of bad
	cerr(str(s)+m);
}
void _err(char const* s, char const* m){
	cerr(str(s)+m);
	throw;
};

thread_local hash_t frand_seed= 1;


}
