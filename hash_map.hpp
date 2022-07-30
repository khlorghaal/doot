#pragma once
#include "vector.hpp"
#include "array_algos.hpp"

#include "hash.hpp"

namespace doot{
	
//all pointers returned are invalidated upon next nonconst method invocation
//buckets of fixed depth, expands once a bucket overflows
template<typename K, typename V>
struct hash_map: container{
	struct slot{
		V v;
		K k;
		bool empty;
	};
	#define SLOT_SIZE sizeof(slot)
	static constexpr ui64 INIT_LEN= 0x20;
	static constexpr i8 GROW_FACTOR= 2;
	static constexpr i16 DEPTH= 8;
	
	arr<slot> heap;
	sizt entry_count=0;//only for debug and profiling
	slot* begin(){ return heap.base; };
	slot* end(){ return heap.stop; };

	hash_map(sizt init_len);
	hash_map();
	~hash_map();

	sizt expand_reserve= 2;//upon expansion, the number of ensured free slots
	//adjustable member so maps can choose smol:greed
	//recommend 1:2:3 :: lazy:normal:greedy
	void expand();

	sizt bucket(K k) const{ return hash(k)%(heap.size()/DEPTH); };
	//return null if not contained
	V* operator[](K k) const;
	
	V* _alloc(K k);

	//places a key and invokes ctor(E...)
	//V* put(K k); removed due to unsafeness
	template<typename... E>
	V& make(K k, E... v);
	//return bool was contained
	bool remove(K k);

	//remove all elements
	void clear();

	void keys_cpy(vector<K>&);
	void values_cpy(vector<V>&);
	void key_values_cpy(vector<pair<K,V>>&);
};



template<typename K, typename V>
hash_map<K,V>::hash_map(sizt init_len){
	ass(DEPTH>=2);
	heap= alloc<slot>(init_len*DEPTH);
	for(auto& e : heap)
		e.empty= true;
}
template<typename K, typename V>
hash_map<K,V>::hash_map(): hash_map(INIT_LEN){
};
template<typename K, typename V>
hash_map<K,V>::~hash_map(){
	free(heap);
}

template<typename K, typename V> 
void hash_map<K,V>::expand(){
	if(expand_reserve<=0){//must increase or is pointless
		bad("hash_map::expand() this.expand_reserve must be >0, to increase the number of free slots");
		expand_reserve= 1;
	}
	if(expand_reserve>DEPTH-1){
		bad("hash_map::expand() this.expand_reserve must be < DEPTH-1, or expansion will bottom");
		expand_reserve= DEPTH-1;
	}

	//move all !null slots
	arr<slot> t_entries= alloc<slot>(heap.size());
	{
		int i=0;
		for(auto& e: heap){
			if(!e.empty)//todo 0can skip rest of bucket
				copy<slot>(t_entries[i++], e);
		}
	}

	sizt nbucks= heap.size()/DEPTH;
	
check_depth:
	//grow until <= DEPTH-RESERVE collisions occur on any bucket
	nbucks*= GROW_FACTOR;
	ass(DEPTH<ui8(-1));
	arr<ui8> bcounts= alloc<ui8>(nbucks);
	fill<ui8>(bcounts, 0);
	for(auto& entry: t_entries){
		auto h= hash(entry.k);
		auto& c= bcounts[h%nbucks];
		c++;//increment bucket's member count
		if(c+expand_reserve>DEPTH){//reserve number of free slots
			free(bcounts);
			goto check_depth;
		}
	}
	free(bcounts);

	//reallocate and set nulls
	free(heap);
	heap= alloc<slot>(nbucks*DEPTH);

	ass(heap.size()%DEPTH==0);
	for(auto& e: heap)
		e.empty= true;

	//repopulate
	for(auto& entry: t_entries)
		copy<V>(*_alloc(entry.k), entry.v);

	free(t_entries);
}

template<typename K, typename V> 
V* hash_map<K,V>::operator[](K k) const{
	sizt i= bucket(k)*DEPTH;
	sizt b= 0;
	while(b!=DEPTH){
		slot& at= heap[i+b++];
		if(at.empty)//not contained
			return (V*)0;
		if(at.k==k)
			return &at.v;
	}
	//not contained
	return (V*)0;
}

template<typename K,typename V>
V* hash_map<K,V>::_alloc(K k){
put_again:
	sizt i= bucket(k)*DEPTH;
	sizt b= 0;
	while(b!=DEPTH){
		slot& at= heap[i+b++];
		if(at.empty){//empty slot found
			new(&at.k)K(k);//for nonprimitive keys such as str
			at.empty= false;
			entry_count++;
			return &at.v;
		}
		else if(at.k==k){//entry with key already present
			ass(false);//i just want to see when this happens
			at.v.~V();
			return &at.v;
		}
	}
	//could not fit in bucket
	expand();
	goto put_again;
	//expand leaves >=1 free slot,
	//thus will not recurse more than once
	return null;
}


template<typename K, typename V> 
template<typename... E>
V& hash_map<K,V>::make(K k, E... e){
	//ppack will handle move ctor properly
	//do not confuse c++ template ppack with c variadic function
	return *new(_alloc(k))V(e...);
}

template<typename K, typename V> 
bool hash_map<K,V>::remove(K k){
	sizt i= bucket(k)*DEPTH;
	sizt b= 0;
	while(b<DEPTH){
		slot& at= heap[i+b++];
		if(at.empty)//not contained; !nulls are always precede nulls in the bucket array
			return false;
		if(at.k==k){//match
			sizt b2= DEPTH-1;
			//start at end, walking back until finding
			//a bucket that is non null. may be self
			slot* swap;
			do swap= &heap[i+b2--];
			while(swap->empty);
			//there will not be a scenario where
			//b2<b; as at must be non null
			at.k= swap->k;
			at.v= swap->v;
			at.empty= swap->empty;
			swap->empty= true;
			entry_count--;
			return true;
		}
	}
	//not contained
	return false;
}

template<typename K, typename V> 
void hash_map<K,V>::clear(){
	for(auto& e: heap)
		e.empty= true;
	entry_count= 0;
}

template<typename K, typename V> 
void hash_map<K,V>::keys_cpy(vector<K>& r){
	for(auto& s: heap)
		if(!s.empty)
			r<<s.k;
}
template<typename K, typename V> 
void hash_map<K,V>::values_cpy(vector<V>& v){
	for(auto& s: arr<slot>(begin(),end()))
		if(!s.empty)
			v<<s.v;
}

}