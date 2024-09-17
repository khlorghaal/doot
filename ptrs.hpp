#include "idheap.hpp"

//idpointers will never be indexes
//as pointy types are persistent, while indices must not persist outside a system op
//may not upcast to id types

//for non-entities
tpl<typn T> struct idpt{
	id i= nullid;
	maybe<T> op()(){ re T::heap[i]; };
	T& un(){ re op()().un(); }
	bool op==(idpt i_){ re i==i_.i; };
	bool op==(id   i_){ re i==i_  ; };
	bool op!(){ re i==nullid; }

	idpt& op=(idpt i_){ i= i_.i; reth; }
};

////necessary from cyclic dependence
//#define IDPT_HEAP_ASSIGN(T) idpt<T>::heap= T::heap;


tpl<typn T, idheap<idheap<T>>& h= T::heap> struct idptpt{
	id i0= nullid;
	id i1= nullid;

	maybe<T> op()(){
		if(op!())
			nope;
		maybe<idheap<T>> hi= h[i0];
		if(!hi)
			nope;
		re hi.un()[i1];
	}
	T& un(){ re op()().un(); }

	bool op!(){ re i0==nullid||i1==nullid; }
};

tplt struct sbpt{
	id i= nullid;
	maybe<T> op()(){ re T::heap[i]; }
	T& un(){ re op()().un(); }
	bool op==(sbpt i_){ re i==i_.i; };
	bool op==(id      i_){ re i==i_  ; };
	bool op!(){ re i==nullid; }

	sbpt& op=(sbpt i_){ i= i_.i; reth; }
	sbpt& op=(id   i_){ i= i_  ; reth; }
};

tplt struct bpt{
	id i;
	//todo idk if this even makes sense
	//needs runtime container ref
};
