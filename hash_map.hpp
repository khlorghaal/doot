#pragma once
#include "vector.hpp"
#include "array_algos.hpp"

#include "hash.hpp"

namespace doot{

struct void_struct{};//for void value

//all pointers returned are invalidated upon next nonconst method invocation
//buckets of fixed depth, expands once a bucket overflows
template<typename K, typename V>
struct hash_map{
	struct slot{
		V v;
		K k;
		bool null;
	};
	#define SLOT_SIZE sizeof(slot)
	static constexpr size_t INIT_LEN= 0x20;
	static constexpr size_t GROW_FACTOR= 4;
	static constexpr size_t DEPTH= 4;

	size_t len;//number of buckets
	slot* heap;
	size_t entry_count=0;//only for debug and profiling
	slot* begin(){ return heap; };
	slot* end(){ return heap+(len*DEPTH); };

	hash_map(size_t init_len);
	hash_map();
	~hash_map();

	void expand();

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
	len= init_len;
	size_t memsiz= len*DEPTH*SLOT_SIZE;
	heap= (slot*)malloc(memsiz);
	memset(heap,1,memsiz);
}
template<typename K, typename V>
hash_map<K,V>::hash_map(): hash_map(INIT_LEN){};
template<typename K, typename V>
hash_map<K,V>::~hash_map(){
	if(heap)
		::free(heap);
}

template<typename K, typename V> 
void hash_map<K,V>::expand(){
	//copy all !null slots
	vector<slot> entries(len*DEPTH);
	for(size_t i=0; i!=len*DEPTH; i++){
		slot& slot= heap[i];
		if(!slot.null)
			entries<<slot;
	}
	::free(heap);

	//grow until < DEPTH collisions occur on any bucket
	//< opposed to <= , aggressively ensures free slots
	size_t nlen= len;
check_depth:
	nlen*= GROW_FACTOR;
	if(nlen*DEPTH*SLOT_SIZE>TOO_BIG)
		throw;
	arr<char> bcounts= alloc<char>(nlen);
	fill<char>(bcounts, 0);
	for(auto& entry: entries){
		hash_t hashv= hash(entry.k);
		size_t hashm= hashv%nlen;
		if(++bcounts[hashm]==DEPTH){
			free(bcounts);
			goto check_depth;
		}
	}
	free(bcounts);
	len= nlen;

	//reallocate and set nulls
	size_t size= len*DEPTH*SLOT_SIZE;
	heap= (slot*)malloc(size);
	if(!heap)
		throw;
	for(int i=0; i!=len*DEPTH; i++)
		heap[i].null= true;

	//repopulate
	for(auto& entry: entries)
		*put(entry.k)= entry.v;
}

template<typename K, typename V> 
V* hash_map<K,V>::operator[](K k) const{
	hash_t h= hash(k);
	size_t i= (h%len)*DEPTH;
	size_t b= 0;
	while(b!=DEPTH){
		slot* at= heap+i+b++;
		if(at->null)//not contained
			return (V*)0;
		if(at->k==k)
			return &at->v;
	}
	//not contained
	return (V*)0;
}

template<typename K, typename V> 
//places a key without setting its value
V* hash_map<K,V>::put(K k){
	assert(!operator[](k));//must not be contained
	hash_t h= hash(k);
	size_t i= (h%len)*DEPTH;
	size_t b= 0;
	while(b!=DEPTH){
		slot& at= heap[i+b++];
		if(at.null){//empty slot found
			at.k= k;
			at.null= false;
			entry_count++;
			return &at.v;
		}
	}
	//could not fit in bucket
	expand();
	return put(k);//expand leaves >=1 free slot,
		          //thus will not recurse more than once
}
template<typename K, typename V> 
bool hash_map<K,V>::remove(K k){
	hash_t h= hash(k);
	size_t i= (h%len)*DEPTH;
	size_t b= 0;
	while(b<DEPTH){
		slot& at= heap[i+b++];
		if(at.null)//not contained; !nulls are always precede nulls
			return false;
		if(at.k==k){//match
			size_t b2= DEPTH-1;
			//start at end, walking back until finding
			//a bucket that is non null. may be self
			slot* swap;
			do swap= heap+i+b2--;
			while(swap->null);
			//there will not be a scenario where
			//b2<b; as at must be non null
			at.k= swap->k;
			at.v= swap->v;
			at.null= swap->null;
			swap->null= true;
			entry_count--;
			return true;
		}
	}
	//not contained
	return false;
}

template<typename K, typename V> 
void hash_map<K,V>::clear(){
	for(slot* i= heap; i!=heap+(len*DEPTH); i++)
		i->null= true;
	entry_count= 0;
}

template<typename K, typename V> 
void hash_map<K,V>::keys_cpy(vector<K>& r){
	for(auto& s: arr<slot>(begin(),end()))
		if(!s.null)
			r<<s.k;
}
template<typename K, typename V> 
void hash_map<K,V>::values_cpy(vector<V>& v){
	for(auto& s: arr<slot>(begin(),end()))
		if(!s.null)
			v<<s.v;
}

}