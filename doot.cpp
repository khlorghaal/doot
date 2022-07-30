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

console cout;
console cerr;
extern void _printf(char const*);
console& console::operator()(str const& x){
	_printf(x.cstr());
	retthis;
};

}
