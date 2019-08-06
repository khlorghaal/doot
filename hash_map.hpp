#pragma once
#include "vector.hpp"
#include "array_algos.hpp"

#include "hash.hpp"

namespace doot{
	
//all pointers returned are invalidated upon next nonconst method invocation
//buckets of fixed depth, expands once a bucket overflows
template<typename K, typename V>
struct hash_map{
	struct slot{
		V v;
		K k;
		bool empty;
	};
	#define SLOT_SIZE sizeof(slot)
	static constexpr size_t INIT_LEN= 0x20;
	static constexpr size_t GROW_FACTOR= 4;
	static constexpr size_t DEPTH= 4;

	arr<slot> heap;
	size_t entry_count=0;//only for debug and profiling
	slot* begin(){ return heap.base; };
	slot* end(){ return heap.stop; };

	hash_map(size_t init_len);
	hash_map();
	~hash_map();

	void expand();

	size_t bucket(K k) const{ return hash(k)%(heap.size()/DEPTH); };
	//returns null if not contained
	V* operator[](K k) const;

	//places a key without setting its value
	V* put(K k);
	void put(K k, V v){ *put(k)= v; }
	//ret was contained
	bool remove(K k);

	//remove all elements
	void clear();

	void keys_cpy(vector<K>&);
	void values_cpy(vector<V>&);
};



template<typename K, typename V>
hash_map<K,V>::hash_map(size_t init_len){
	assert(DEPTH>=2);
	heap= alloc<slot>(init_len*DEPTH);
	for(auto& e : heap)
		e.empty= true;
}
template<typename K, typename V>
hash_map<K,V>::hash_map(): hash_map(INIT_LEN){};
template<typename K, typename V>
hash_map<K,V>::~hash_map(){
	free(heap);
}

template<typename K, typename V> 
void hash_map<K,V>::expand(){
	//copy all !null slots
	vector<slot> entries(heap.size());
	for(auto& e: heap){
		if(!e.empty)
			entries<<e;
	}
	free(heap);

	//grow until < DEPTH collisions occur on any bucket
	//< opposed to <= , aggressively ensures free slots
	size_t nbucks= heap.size()/DEPTH;
check_depth:
	nbucks*= GROW_FACTOR;
	arr<char> bcounts= alloc<char>(nbucks);
	fill<char>(bcounts, 0);
	for(auto& entry: entries){
		if(++bcounts[bucket(entry.k)]==DEPTH){
			free(bcounts);
			goto check_depth;
		}
	}
	free(bcounts);

	//reallocate and set nulls
	heap= alloc<slot>(nbucks*DEPTH);
	for(auto& e: heap)
		e.empty= true;

	//repopulate
	for(auto& entry: entries)
		*put(entry.k)= entry.v;
}

template<typename K, typename V> 
V* hash_map<K,V>::operator[](K k) const{
	size_t i= bucket(k)*DEPTH;
	size_t b= 0;
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

template<typename K, typename V> 
V* hash_map<K,V>::put(K k){
	size_t i= bucket(k)*DEPTH;
	size_t b= 0;
	while(b!=DEPTH){
		slot& at= heap[i+b++];
		if(at.empty){//empty slot found
			at.k= k;
			at.empty= false;
			entry_count++;
			return &at.v;
		}
		else if(at.k==k)//entry with key already present
			return NULL;
	}
	//could not fit in bucket
	expand();
	return put(k);//expand leaves >=1 free slot,
		          //thus will not recurse more than once
}

template<typename K, typename V> 
bool hash_map<K,V>::remove(K k){
	size_t i= bucket(k)*DEPTH;
	size_t b= 0;
	while(b<DEPTH){
		slot& at= heap[i+b++];
		if(at.empty)//not contained; !nulls are always precede nulls
			return false;
		if(at.k==k){//match
			size_t b2= DEPTH-1;
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