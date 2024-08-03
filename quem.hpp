#include "idheap.hpp"
#include "thread.hpp"
namespace doot{

//threadsafe queue
tplt class quem{
	mutex mut;
	idheap<arr<T>> heap;
	siz tail= 0;
	siz head= 0;
public:
	tples void push(E cst&... e){
		mut.lock();
		heap.add(head++,e...);
		mut.locknt();
	}
	maybe<T&> pull(T& def){
		mut.lock();
		auto r= head<tail ?
			maybe<T&>::yes(heap[tail++]):
			maybe<T&>::no();
		mut.locknt();
		retr;
	}
};

}