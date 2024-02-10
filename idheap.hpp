#pragma once
#include "doot.hpp"
#include "array_algos.hpp"

namespace doot{
/*
Nonconst method invocations invalidate pointers.
TODO use table instead of vector
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
	static constexpr sizt INIT_CAP= 0x20;
	static constexpr sizt GROW_FACTOR= 4;

	vector<T> heap;
	vector<id> ids;//index->id. element-aligned with heap
	vector<idx> map;//id->index. associative array.

	arrayable( heap.base, heap.stop );
	//arr<id> id_iter(){ return ids; };

	idheap(sizt init_cap);
	idheap(): idheap(8){};
	~idheap()= default;
	idheap(idheap&& b)= default;//vector move ctor invoked

	template<typename... E>
	T& add(id, E const& ...);

	//sub for pointer, id, or object-equality
	void sub(id);
	id sub(T* t);
	id sub(T& t);

	idx index(id) const;
	id ptr_id(T* t) const{
		sizt i= t-heap.base;
		ass(i>=0&&i<heap.size());//boundcheck
		return ids[i];
	}
	T* operator[](id) const;
	T& getormake(id);
	void getarr(arr<id>,arr<T*>);

	void purge();
};

//convenient, type-safe
tpl<typn T, typn I= id>
struct idptr{
	I i;
	idptr(): i(NULLID){};
	idptr(I i_): i(i_){};
	operator I(){ return i; }
	void operator=(I i_){ i= i_; }
	T& operator*();
	T* operator->(){ return &(operator*()); };
	bool operator!(){ return i==NULLID; };
};
tplt struct  eidptr: idptr<T,eid>{};
tplt struct ecidptr: idptr<T,ecid>{};

//static heap only
tplt struct hidptr: idptr<T>{
	inline static idheap<T>& h= T::heap;
	using idptr<T>::i;

	T& operator*(){	return *h[i]; }
	T* operator->(){return  h[i]; }
	bool operator!(){ return (i==NULLID); }
};

#define ZIP_HEAP(o,id_,h) \
auto& _lh= h.heap;\
auto& _li= h.ids;\
for(idx _i##o=0; _i##o!=_lh.size(); _i##o++)\
	for(    auto&   o= _lh[_i##o];0;)\
	for(id const& id_= _li[_i##o];0;)


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
	vector<idx> freed;
	idx next= 0;

	idx pop(){
		if(freed.empty())
			return next++;
		else
			return *(--freed.stop);
	}
	inline idx operator()(){ return pop(); }

	void free(idx i){
		ass(i<next);
		freed.add(i);
	}
};

/*not static
intended for
	-components which need local heap
	-static types which shouldnt use entities*/
tpl<typn T> struct bag: idheap<T>{
	index_recycler rcyc;

	tpl<typn... E>
	T& add(E const&... e){
		return idheap<T>::add(rcyc(),e...);
	}
	
	void sub(id cid){
		idheap<T>::sub(cid);
		rcyc.free(cid);
	};
	void sub(T& e){
		rcyc.free(idheap<T>::sub(e));
	}
	OPADDSUB;
};

//static bag heap
//crtp only
tplt struct bheap: bag<T>{
	inline static bag<T> heap;
	using ptr= class idptr<T>;
	ptr self;

	tpl<typn... E>
	static T& add(E... e){
		id i= bag<T>::rcyc();
		T& r= idheap<T>::add(i,e...);
		r.self= i;
		return r;
	}
	operator ptr(){ return self; }
	bool operator!(){ return self==nullid; }
};

//for classes of
//	static type::heap
#define HEAP_PTR_DEREF_DEF(T) \
tpl<> T& idptr<T>::operator*(){\
	return *(T::heap[i]);\
}

tplt idheap<T>::idheap(sizt init_cap){
	heap.realloc(init_cap);
	 ids.realloc(init_cap);
	 map.realloc(init_cap);
	map.stop= map.cap;
	fill(map, NULLIDX);
}

tplt template<typename... E>
T& idheap<T>::add(id id, E const& ... e){
	ass(id!=NULLID);
	sizt mapsiz= map.size();
	if(id>=mapsiz){//expand map
		if(id>TOO_BIG)
			err("max id exceded");
		sizt nmapsiz= (id*vector<T>::GROW_FACTOR);
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

	return ret;
}

tplt void idheap<T>::sub(id i){
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
	return i;
};
tplt id idheap<T>::sub(T& t){
	EN(i,e,heap){
		if(e==t){
			id id= ids[i];
			sub(id);
			return id;
		}
	}
}

tplt idx idheap<T>::index(id id) const{
	if(id<0)
		bad("idheap::idx<0");
	if(id>=map.size())
		return NULLIDX;
	return map[id];
}
tplt T* idheap<T>::operator[](id id) const{
	sizt idx= index(id);
	if(idx==NULLIDX)
		return (T*)0;
	return &heap[idx];
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
template<typename T,sizt I>
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
template<typename T, sizt I>
multimapped_heap<T,I>::multimapped_heap(){
	constexpr sizt init_cap= 0x20;
	eids.realloc(init_cap);
}
template<typename T, sizt I>
multimapped_heap<T,I>::~multimapped_heap(){
	for(auto& o : *this)
		o.~T();
}
template<typename T, sizt I>
T& multimapped_heap<T,I>::make(eid eid){
	cid a;
	return make(eid,a);
}
template<typename T, sizt I>
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
template<typename T, sizt I>
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
template<typename T, sizt I>
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


template<typename T, sizt I>
eid multimapped_heap<T,I>::cid2eid(cid cid) const{
	idx idx= heap.index(cid);
	if(idx==NULLIDX)
		return NULLIDX;
	return eids[idx];
};
template<typename T, sizt I>
T* multimapped_heap<T,I>::cid2t(cid cid) const{
	return heap[cid];
};
template<typename T, sizt I>
arr<cid> multimapped_heap<T,I>::eid2cids(eid eid) const{
	auto rp= map[eid];
	if(!!rp)
		ass(rp->size()<=I);
	return !!rp? *rp : EMPTY<cid>;
};
template<typename T, sizt I>
dynarr<T*,I> multimapped_heap<T,I>::operator[](eid eid) const{
	arr<cid> cids= eid2cids(eid);
	dynarr<T*,I> ret;
	for(auto& cid : cids)
		ret.make( cid2t(cid) );
	return ret;
};
*/

}