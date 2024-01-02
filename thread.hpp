/*
synchro primitives are not exposed
all interthread via
	warp dispatch/gather
	message queues

message queues explicitly consumed within thread loop
	allows easier strong type
*/

#pragma once
#include "doot.hpp"
#include "string.hpp"
#include "vector.hpp"

namespace doot{

/*doot threading notions constancy
	threads may not terminate
	termination happens with process termination
*/
CALL_T(thread_arg,void*,void);
void thread(str name, thread_arg);

struct mutex{
	OPAQ_M(_);
	OPAQ_CDTOR_DECL(mutex);
	void lock();
	void locknt();
};

//includes own mutex and predicate. does not wake spuriously
struct lock{
	OPAQ_M(_);
	OPAQ_CDTOR_DECL(lock);
	void wait();//multiple threads may await
	void wake();//all threads waiting
};

//countdown semaphore
struct latch{
	OPAQ_M(_);
	OPAQ_CDTOR_DECL(latch);
	void  set(int count);
	void tick();
	void wait();
};

/*one warp per process, unlike gpu warps
warps are invoked sequentially from control thread
threads - init; serial; dispatch; task; gather; task; ...
        - idle;                   task; idle;   task; ...
        - idle;                   task; idle;   task; ...
        - idle;                   task; idle;   task; ...
*/
namespace warp{

bool constexpr WARP_NO_MULTITHREAD= true;
extern int poolsize;//excludes invoking working thread
extern volatile bool active;

void init();

//allows the implementation to omit headers
extern void _dispatch(
	i8* base, i8* start, sizt stride,
	FPTR(task,void*,void));

/*
uses the calling thread + pool
blocks until completion
In Place Transform pattern

each thread is given an arr<T>
	handled by the system
warp itself does not handle iteration
	because perf reasons

void task(arr<T>)
	for(mut T in arr)
		stuff(t)
passed as function ptr

jobs must not be reallocated in any way until complete
	if scoping properly is nonconcern
*/
template<typename T, FPTR(task,arr<T>,void)>
inline void dispatch(arr<T> jobs){
	_invoke(jobs.base,jobs.stop, TSIZ,
		//(arr<T> -> void) -> (void* -> void)
		&voidpfunct<arr<T>,task> );
}

}
}
