#pragma once
#include <cstring>
#include "vector.hpp"

#include "hash.hpp"

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
	static constexpr size_t GROW_FACTOR= 8;
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
	//ret was contained
	bool remove(K k);

	//remove all elements
	void clear();

	void keys_cpy(vector<K>&);
	void values_cpy(vector<V>&);
};



template<typename K, typename V>
hash_map<K,V>::hash_map(size_t init_len){
	#ifndef NDEBUG
	if(DEPTH<2)
		throw;
	#endif
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
		free(heap);
}

template<typename K, typename V> 
void hash_map<K,V>::expand(){
	//copy all !null slots
	slot* entries= (slot*)malloc(len*DEPTH*SLOT_SIZE);
	if(!entries)
		throw;
	size_t entry_count= 0;
	for(size_t i=0; i!=len*DEPTH; i++){
		slot& slot= heap[i];
		if(!slot.null)
			entries[entry_count++]= slot;
	}

	//grow until < DEPTH collisions occur on any bucket
	//< opposed to <= , aggressively ensures free slots
	size_t nlen= len;
check_depth:
	nlen*= GROW_FACTOR;
	char* bcounts= (char*)calloc(nlen,1);
	for(size_t i=0; i!=entry_count; i++){
		char& bcount= bcounts[hash(entries[i].k)%nlen];
		if(++bcount==DEPTH){
			free(bcounts);
			goto check_depth;		
		}
	}
	free(bcounts);
	len= nlen;

	//reallocate and set nulls
	size_t size= len*DEPTH*SLOT_SIZE;
	free(heap);
	heap= (slot*)malloc(size);
	if(!heap)
		throw;
	memset(heap,1,size);

	//repopulate
	for(size_t i=0; i!=entry_count; i++){
		slot& s= entries[i];
		*put(s.k)= s.v;
	}
	free(entries);
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
	#ifndef NDEBUG
	if(operator[](k))//must not be contained
		throw;
	#endif
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
			r<<s.v;
}