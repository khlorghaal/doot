#include "idheap.hpp"
namespace doot{

//idpointers will never be indexes
//as pointy types are persistent, while indices must not persist outside a system op
//may not upcast to id types
//shares interface with maybe
tpl<typn T> struct idpt{
	id i= nullid;

	bool op!(){ re i==nullid; }
	op bool(){ re !op!();}

	idpt(id i_):i(i_){}
	idpt():i(nullid){}
	tple idpt(maybe<component<E>> m):i(
		m?(m.un().self()):nullid
		){}
	op id(){ re  i ; }
	idpt& op=(idpt i_){ i= i_.i; reth; }
	
	maybe<T> op()( ){ re T::heap[i]; };//pt=>maybe
	T& op()(T& none){ re T::heap[i](none); }//pt=>maybe=>T
	T& un(){ re op()().un(); }//pt=>T
	bool op==(idpt i_){ re i==i_.i; };
	bool op==(id   i_){ re i==i_  ; };

};

tpl<typn T, idheap<idheap<T>>& h= T::heap> struct idptpt{
	//todo is this shit?
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

	
}