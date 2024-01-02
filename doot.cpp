#include "doot.hpp"

//preinclude to have macros work
#include "string.hpp"
#include "bytebuf.hpp"
#include "timer.hpp"
#include "math.hpp"
#include "thread.hpp"
#include "file.hpp"

#include "tests.impl.hpp"

#include "string.impl.hpp"
#include "bytebuf.impl.hpp"
#include "timer.impl.hpp"
#include "trans2d.impl.hpp"
#include "thread.impl.hpp"

namespace doot{

void warn(char const* m){
	cerr(m);
}
void bad(char const* m){
	cerr(m);
}
void err(char const* m){
	cerr(m);
	throw;
};

thread_local hash_t frand_seed= 1;


}
