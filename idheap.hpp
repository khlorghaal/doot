#pragma once
#include "doot.hpp"
#include "index_recycler.hpp"
#include "array_algos.hpp"

namespace doot{
/*
Nonconst method invocations invalidate pointers.
TODO use table instead of vector
*/

/*
id->idx->T
Uses an associative array of id->idx
to map into an unsorted gapless heap of T.

ID's are allocated externally, ideally by an index_recycler
*/
tplt
struct idheap: container{
	static constexpr size_t INIT_CAP= 0x20;
	static constexpr size_t GROW_FACTOR= 4;

	vector<T> heap;
	vector<id> ids;//index->id. element-aligned with heap
	vector<idx> map;//id->index. associative array.

	arrayable( heap.base, heap.stop );
	//arr<id> id_iter(){ return ids; };

	idheap(size_t init_cap);
	idheap(): idheap(8){};
	~idheap()= default;
	idheap(idheap&& b)= default;//vector move ctor invoked

	template<typename... E>//e includes value category
	T& make(id,E...);
	//T& put(id i, T&& e){ return put<T&&>(i,(T&&)e); }
	//T& make(id i){ return put<T&&>(i,T()); }

	void kill(id);//calls dtor
	void kill(T* t){ kill(ptr_id(t)); };

	idx index(id) const;
	id ptr_id(T* t) const{
		size_t i= t-heap.base;
		ass(i>=0&&i<heap.size());
		return ids[i];
	}
	T* operator[](id) const;
	T& getormake(id);
	void getarr(arr<id>,arr<T*>);

	void purge();
};

template<typename T, idheap<T>& h_T= T::heap>
struct idptr_heap{
	inline static auto& h= h_T;
	id _;
	idptr_heap(): id(NULLID){};
	idptr_heap(id id): _(id){};

	operator id(){ return _; }
	void operator=(id id){ _= id; }
	T& operator*(){	return *h[_]; }
	T* operator->(){ return h[_]; }
	bool operator!(){ return (_==NULLID)||(!h[_]); }
};

#define zip_heap(o,id,h) {\
auto& _lh= h.heap;\
auto& _li= h.ids;\
for(int _i=0; _i!=_lh.size(); _i++){\
	auto&  o= _lh[_i];\
	auto& id= _li[_i];


#define zip_multiheap(o,id,h) {\
auto& _lh= h.heap.heap;\
auto& _li= h.eids;\
for(int _i=0; _i!=_lh.size(); _i++){\
	auto&  o= _lh[_i];\
	auto& id= _li[_i];





template<typename T>
idheap<T>::idheap(size_t init_cap){
	heap.realloc(init_cap);
	 ids.realloc(init_cap);
	 map.realloc(init_cap);
	map.stop= map.cap;
	fill(map, NULLIDX);
}

template<typename T>
template<typename... E>
T& idheap<T>::make(id id, E... n){
	ass(id!=NULLID);
	size_t mapsiz= map.size();
	if(id>=mapsiz){//expand map
		if(id>TOO_BIG)
			err("max id exceded");
		size_t nmapsiz= (id*vector<T>::GROW_FACTOR);
		map.realloc(nmapsiz);
		map.stop= map.cap;
		fill({map.base+mapsiz, map.stop}, NULLIDX);
	}
	if(map[id]!=NULLIDX)//entry already present
		err(str::fmt("entry already present %i - %i",id,map[id]).cstr());

	ass(ids.size()==heap.size());
	size_t idx= heap.size();
	T& e= heap.make((E...)n...);//cast for xvalue fuckery
	ids.make(id);
	map[id]= idx;

	return e;
}

template<typename T>
void idheap<T>::kill(id i){
	idx x= index(i);
	if(x==NULLIDX)
		return;

	ass(ids.size()==heap.size());
	idx idx_end= heap.size()-1;
	id id_end= ids[idx_end];

	heap[x].~T();//dlet
	map[i]= NULLIDX;//unmap id

	if(x!=idx_end){
		//swap with end entry
		//heap[x]= (T&&)heap[idx_end];//object
		__memcpy(&heap[x], &heap[idx_end],TSIZ);

		 ids[x]= ids[idx_end];//id
		map[id_end]= x;//remap end index
	}
	heap.stop--;
	ids.stop--;
}

template<typename T>
idx idheap<T>::index(id id) const{
	if(id<0)
		throw;
	if(id>=map.size())
		return NULLIDX;
	return map[id];
}
template<typename T>
T* idheap<T>::operator[](id id) const{
	size_t idx= index(id);
	if(idx==NULLIDX)
		return (T*)0;
	return &heap[idx];
}
template<typename T>
T& idheap<T>::getormake(id id){
	idx idx= index(id);
	return idx==NULLIDX? make(id):heap[idx];
}
template<typename T>
void idheap<T>::getarr(arr<id> in, arr<T*> out){
	zip(i,o,in,out){
		o= operator[](i);
	}}
}
template<typename T>
void idheap<T>::purge(){
	heap.clear();
	ids.clear();
	fill(map, NULLIDX);
}








/*
REMOVED in favor of references of {eid,cid}
where each entity has its own idheap

maps from entity-id into an array of
component-id pointers, which map onto T.

generally you will just want a component with an array instead,
as a group will generally have some data which needs forwarded,
and update the elements when forwarding

never store the index of a component within its group,
instead use CIDs, as the group's dynarr swaps upon removals

stores a relation of cid->eid, mainly for use in destruction.
a T may not have its entity changed,
however T& s may have their data swapped
*/
/*
template<typename T,size_t I>
struct multimapped_heap: container{
	arrayable(heap.begin(),heap.end());
	idheap<T> heap;// component-id -> index -> &T
	vector<eid> eids;// component-id -> index -> EID
	idheap<dynarr<cid,I>> map;// entity-id -> component-id[]

	index_recycler cidget;

	multimapped_heap();
	~multimapped_heap();

	T& make(eid eid,cid& cid);
	T& make(eid eid);
	void killAll(eid id);
	void kill(cid id);

	eid cid2eid(cid cid) const;
	T* cid2t(cid cid) const;
	arr<cid> eid2cids(eid eid) const;
	dynarr<T*,I> operator[](eid cid) const;
};
template<typename T, size_t I>
multimapped_heap<T,I>::multimapped_heap(){
	constexpr size_t init_cap= 0x20;
	eids.realloc(init_cap);
}
template<typename T, size_t I>
multimapped_heap<T,I>::~multimapped_heap(){
	for(auto& o : *this)
		o.~T();
}
template<typename T, size_t I>
T& multimapped_heap<T,I>::make(eid eid){
	cid a;
	return make(eid,a);
}
template<typename T, size_t I>
T& multimapped_heap<T,I>::make(eid e, cid& c){
	//allocate into heap
	c= cidget();
	idx idx= heap.heap.size();
	T& r= heap.make(c);

	//idx -> eid map
	ass(idx==eids.size());
	eids<<e;

	//find or allocate eid -> arr<cid> map
	dynarr<cid,I>* vp= map[e];
	if(!vp)
		vp= &map.make(e);
	if(vp->size()==I)
		throw;
	vp->make(c);

	return r;
}
template<typename T, size_t I>
void multimapped_heap<T,I>::killAll(eid eid){
	dynarr<cid,I>* vp= map[eid];
	if(!vp){
		error("eid group "<<eid<<" unpresent");
		return;
	}
	dynarr<cid,I> vc= *vp;//must copy because cmod
	for(cid cid : vc)
		kill(cid);
}
template<typename T, size_t I>
void multimapped_heap<T,I>::kill(cid c){
	ass(c!=NULLID);
	
	//remove from heap
	idx hidx= heap.index(c);
	if(hidx==NULLIDX)
		error(strfmt("cid %i unpresent",c));
	heap.kill(c);

	//remove from eidmap
	eid e= eids[hidx];
	ass(e!=NULLID);
	eids.remove_idx(hidx);

	//remove from map
	dynarr<cid,I>* cidlist_p= map[e];
	ass(!!cidlist_p);
	dynarr<cid,I>& cidlist= *cidlist_p;
	idx cidlist_idx= find<cid>(cidlist, c);
	ass(cidlist_idx!=NULLIDX);
	cidlist.rem(cidlist_idx);

	//remove cidlist if empty
	if(cidlist.size()==0)
		map.kill(e);
}


template<typename T, size_t I>
eid multimapped_heap<T,I>::cid2eid(cid cid) const{
	idx idx= heap.index(cid);
	if(idx==NULLIDX)
		return NULLIDX;
	return eids[idx];
};
template<typename T, size_t I>
T* multimapped_heap<T,I>::cid2t(cid cid) const{
	return heap[cid];
};
template<typename T, size_t I>
arr<cid> multimapped_heap<T,I>::eid2cids(eid eid) const{
	auto rp= map[eid];
	if(!!rp)
		ass(rp->size()<=I);
	return !!rp? *rp : EMPTY<cid>;
};
template<typename T, size_t I>
dynarr<T*,I> multimapped_heap<T,I>::operator[](eid eid) const{
	arr<cid> cids= eid2cids(eid);
	dynarr<T*,I> ret;
	for(auto& cid : cids)
		ret.make( cid2t(cid) );
	return ret;
};
*/

}