#pragma once
#include "doot.hpp"
#include "algos.hpp"

namespace doot{
/*
Noncst method invocations invalidate pointers.
TODO use table instead of list
*/

/*
ECS primary structure
id->idx->T
Uses an associative array of id->idx
to map into an unsorted gapless heap (bag) of T.

is basically a specialized map<id,T>, where ids are small coherent numbers

IDs are allocated externally, ideally by an index_recycler
*/
tplt struct idheap: container{
	sex siz INIT_CAP= 0x20;
	sex siz GROW_FACTOR= 4;

	list<T > heap;
	list<id> ids;//index->id. element-aligned with heap
	list<maybe_i<idx>> map;//id->index. associative array.

	arrayable( heap.base, heap.stop );
	//arr<id> id_iter(){ re ids; };

	idheap(sizt init_cap);
	idheap(): idheap(INIT_CAP){};
	~idheap()= default;
	//idheap(idheap&& b)= default;//list move ctor invoked

	tples T& add(id, E&&...);

	//sub for pointer, id, or object-equality
	void sub(id);

	mai<idx> index(id) cst;//generally private
	may<T> op[](id) cst;
	T& getormake(id);

	void purge();

	void prealloc(siz);
};

#define zip_heap(o,id_,h) \
	let(are _lh= h.heap)\
	let(are _li= h.ids)\
	ra(_i##o,_lh.size())\
		let(are   o=     _lh[_i##o])\
		let(acr id_= (id)_li[_i##o])
//distinct from en, as is not sequence

//#define ZIP_MULTIHEAP(o,id,h) {\
//auto& _lh= h.heap.heap;\
//auto& _li= h.ids;\
//for(int _i=0; _i!=_lh.size(); _i++){\
//	auto&  o= _lh[_i];\
//	auto& id= _li[_i];


/*
returns an unused index
keeps a list of freed indices which it recycles first,
if none are free it makes a new one

popped indices will have closely adjacent values,
making them suited for associative arrays
*/
struct index_recycler{
	list<idx> freed;
	idx next= 0;

	idx pop(){
		if(freed.empty())
			re next++;
		else
			re *(--freed.stop);
	}
	inl idx op()(){ re pop(); }

	void free(idx i){
		ass(i<next);
		freed.add(i);
	}
};

//not static
//intended for local heaps on objects
tplt struct bag: idheap<T>{
	index_recycler rcyc;

	tples pair<T&,id> add(E&&... e){
		id i= rcyc();
		re {idheap<T>::add(i,e...),i};}
	
	void sub(id cst cid){
		idheap<T>::sub(cid);
		rcyc.free(cid);};
	void sub(T& e){
		rcyc.free(idheap<T>::sub(e));}
};

tplt idheap<T>::idheap(siz init_cap){
	heap.realloc(init_cap);
	 ids.realloc(init_cap);
	 map.realloc(init_cap);//heuristic
	map.stop= map.cap;
	fill(map,{});
}

tplt tples
T& idheap<T>::add(id id, E&&... e){
	if(id==nullid){
		bad("nullid added; spicey orphan emitted");
		re *new T(e...);
	}
	siz ms= map.size();
	if(id>=ms){//expand map
		if(id>TOO_BIG){
			bad("max id exceded; spicey orphan emitted");
				re *new T((E&&)e...);
				//why did i think this was a good idea
		}
		siz nms= (id*list<T>::GROW_FACTOR);
		ass(nms>0);
		map.realloc(nms);
		fill({map.stop,map.cap},mai<idx>{});
		map.stop= map.cap;
		ms= nms;
	}
	ass(id<map.size());//THIS
	may_if(idx m,map[id]){//entry already present
		bad("entry already present id=");
		//fukkin deps bad(str("entry already present id=")+id+", idx="+m);
		re heap[m];
	}

	ass(ids.size()==heap.size());
	idx idx= heap.size();
	T&    r= heap.add((E&&)e...);
	ids.add(id);
	map[id]= {idx};

	ass(map[id].un()==idx);
	ass(ids[map[id].un()]==id);

	retr;
}

tplt void idheap<T>::sub(
	       id  cst  id_sub){
	may_re(idx cst idx_sub,index(id_sub));
	       idx cst idx_end= ids.size()-1;//replacement
	       id  cst  id_end= ids[idx_end];
	ass(ids[map[id_end].un()]==id_end);
	
	ass(ids.size()==heap.size());
	ass(idx_sub<heap.size());
	ass(idx_end==map[id_end].un());

	     heap[idx_sub].~T();//dlet
	map[id_sub]= {nullidx};//unmap

	if(likely(id_sub!=id_end)){
			//move end onto freed spot
			copy(heap[idx_sub], heap[idx_end]);
			      ids[idx_sub]= id_end; 
				  map[ id_end]= {idx_sub};

		ass(ids[map[id_end].un()]==id_end);
	}
	ass(ids[idx_sub]==id_end);
	heap.stop--;
	 ids.stop--;
}
/*tplt id idheap<T>::sub(T* t){
	siz x= t-heap.base;
	ass(0<=x&x<heap.size());
	id i= ids[x];
	ass(map[i].un()==x);
	sub(i);
	re i;
};*/

tplt mai<idx> idheap<T>::index(id id) cst{
	if(unlikely( id>=map.size() ))//implies null because unsigned
		nope;
	re map[id];
}
tplt maybe<T> idheap<T>::op[](id id) cst{
	may_nope(ato i,index(id));
	re {&heap[   i]};
}
tplt void idheap<T>::purge(){
	heap.clear();
	ids.clear();
	fill(map,mai<idx>{});
}

tplt void idheap<T>::prealloc(siz s){
	heap.prealloc(s);
	ids.prealloc(s);
	if(siz ms= map.size(); s>ms){
		realloc(map,s);//mapsize is heuristic here
		fill({map.base+ms,map.stop},mai<idx>{});
	}
}

};