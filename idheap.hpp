#pragma once
#include "doot.hpp"
#include "array_algos.hpp"

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

	list<T  > heap;
	list<id > ids;//index->id. element-aligned with heap
	list<idx> map;//id->index. associative array.

	arrayable( heap.base, heap.stop );
	//arr<id> id_iter(){ re ids; };

	idheap(sizt init_cap);
	idheap(): idheap(8){};
	~idheap()= default;
	//idheap(idheap&& b)= default;//list move ctor invoked

	tples T& add(id, E&&...);

	//sub for pointer, id, or object-equality
	void sub(id);
	id sub(T* t);

	may<idx> index(id) cst;
	id ptr_id(T* t) cst{
		sizt i= t-heap.base;
		ass(i>=0&&i<heap.size());//boundcheck
		re ids[i];
	}
	may<T> op[](id) cst;
	T& getormake(id);
	void getarr(arr<id>,arr<T*>);

	void purge();
};

#define ZIP_HEAP(o,id_,h) \
auto& _lh= h.heap;\
auto& _li= h.ids;\
for(idx _i##o=0; _i##o!=_lh.size(); _i##o++)\
	for(    auto&   o= _lh[_i##o];0;)\
	for(id cst& id_= _li[_i##o];0;)


//#define ZIP_MULTIHEAP(o,id,h) {\
//auto& _lh= h.heap.heap;\
//auto& _li= h.eids;\
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

/*not static
intended for
	-components which need local heap
	-static types which shouldnt use entities

todo merge kinds with component*/
tplt struct bag: idheap<T>{
	index_recycler rcyc;

	tples pair<T&,id> add(E&&... e){
		id i= rcyc();
		re {idheap<T>::add(i,e...),i};}
	
	void sub(id cid){
		idheap<T>::sub(cid);
		rcyc.free(cid);};
	void sub(T& e){
		rcyc.free(idheap<T>::sub(e));}
};

//bag but static
tplt struct sbag{
	inl static index_recycler rcyc;
	inl static idheap<T> heap;

	tples static pair<T&,id> add(E&&... e){
		id i= rcyc();
		re { heap.add(i,e...), i};}
	
	static void sub(id cid){
		heap.sub(cid);
		rcyc.free(cid);};
	static void sub(T* e){
		rcyc.free(heap.sub(e));}
};

//killbus per eid
tplt struct component{
	inl static idheap<T> heap;

	//secondary inherited types must procede the component inheritance
	//ordering is vital for intializers
	tples static T& make(eid eid, E&&... e){
		if cex (VOID_VARIAD(E)){
			re heap.add(eid, {
				component<T>{},
				e...});
		}
		else{
			re heap.add(eid);
		};
	}
	static void kill(eid eid){    heap.sub(eid); };
};


tplt idheap<T>::idheap(sizt init_cap){
	heap.realloc(init_cap);
	 ids.realloc(init_cap);
	 map.realloc(init_cap);
	map.stop= map.cap;
	fill(map, NULLIDX);
}

tplt tples
T& idheap<T>::add(id id, E&&... e){
	ass(id!=NULLID);
	sizt mapsiz= map.size();
	if(id>=mapsiz){//expand map
		if(id>TOO_BIG)
			err("max id exceded");
		sizt nmapsiz= (id*list<T>::GROW_FACTOR);
		map.realloc(nmapsiz);
		map.stop= map.cap;
		fill({map.base+mapsiz, map.stop}, NULLIDX);
	}
	if(map[id]!=NULLIDX)//entry already present
		err(strfmt("entry already present %i - %i",id,map[id]));

	ass(ids.size()==heap.size());
	sizt idx= heap.size();
	T& ret= heap.add(e...);
	ids.add(id);
	map[id]= idx;

	re ret;
}

tplt void idheap<T>::sub(id i){
	may<idx> mx= index(i);
	if(!mx)
		re;
	idx x= mx.un();

	ass(ids.size()==heap.size());
	idx idx_end= heap.size()-1;
	id id_end= ids[idx_end];

	heap[x].~T();//dlet
	map[i]= NULLIDX;//unmap id

	if(x!=idx_end){
		//swap with end entry
		//heap[x]= (T&&)heap[idx_end];//object
		_memcpy(&heap[x], &heap[idx_end],TSIZ);

		 ids[x]= ids[idx_end];//id
		map[id_end]= x;//remap end index
	}
	heap.stop--;
	ids.stop--;
}
tplt id idheap<T>::sub(T* t){
	id i= ptr_id(t);
	sub(i);
	re i;
};

tplt may<idx> idheap<T>::index(id id) cst{
	if(id<0 || id>=map.size()){
		warn("idheap::index param:id OOB");
		nope;
	}
	idx& mi= map[id];//list
	ass( mi!=nullidx);
	re { mi};
}
tplt maybe<T> idheap<T>::op[](id id) cst{
	auto idx= index(id);
	if(!idx)
		nope;
	re {heap[idx.un()]};
}
tplt void idheap<T>::getarr(arr<id> in, arr<T*> out){
	ZIP(i,o,in,out){
		o= op[](i);
	}
}
tplt void idheap<T>::purge(){
	heap.clear();
	ids.clear();
	fill(map, NULLIDX);
}

}