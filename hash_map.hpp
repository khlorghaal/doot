#pragma once
#include "vector.hpp"
#include "array_algos.hpp"

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
	static cex siz SLOT_SIZE= sizeof(slot);
	static cex u16 DEPTH= 4;//depth must be cex for performance
	static cex siz DEFAULT_LEN= 0x20;

	u8 grow_factor= 4;//tunable
	u8 expand_pad= 1;
	
	arr<slot> heap;
	sizt entry_count=0;//only for debug and profiling
	slot* begin(){ return heap.base; };
	slot* end(){ return heap.stop; };

	hash_map(sizt init_len);
	hash_map();
	~hash_map();

	sizt expand_reserve= 1;//upon expansion, the number of ensured free slots
	//adjustable member so maps can choose smol:greed
	//recommend 1:2:3 :: lazy:normal:greedy
	void expand();

	sizt bucket(K k) const{ return hash(k)%(heap.size()/DEPTH); };
	//return null if not contained
	V* operator[](K k) const;
	
	V* _alloc(K k);

	//places a key and invokes ctor(E...)
	template<typename... E>
	V& add(K k, E... v);
	//return bool was contained
	bool sub(K k);
	OPADDSUB;

	//remove all elements
	void clear();

	void keys_cpy(vector<K>&);
	void values_cpy(vector<V>&);
	void key_values_cpy(vector<pair<K,V>>&);

};
#define hmap hash_map

//expensive, avoid
#define EACH_HMAP(e,M) \
		EACH(_S_##e,M.heap)\
			if(unlikely(!_S_##e.empty))\
				fauto( e= _S_##e.v )

template<typename K, typename V>
hash_map<K,V>::hash_map(sizt init_len){
	ass(DEPTH>=2);
	heap= alloc<slot>(init_len*DEPTH);
	for(auto& e : heap)
		e.empty= true;
}
template<typename K, typename V>
hash_map<K,V>::hash_map(): hash_map(hash_map<K,V>::DEFAULT_LEN){
};
template<typename K, typename V>
hash_map<K,V>::~hash_map(){
	free(heap);
}

template<typename K, typename V> 
void hash_map<K,V>::expand(){
	if(expand_pad<0)//must increase or is pointless
		err("hash_map::expand() this.expand_reserve must be >0, to increase the number of free slots");
	if(expand_pad>DEPTH-1)
		err("hash_map::expand() this.expand_reserve must =< DEPTH-1, or expansion will bottom");

	//move all !null slots
	arr_raii<slot> t_entries(heap.size());
	siz i= 0;
	EACH(e,heap){
		if(!e.empty){//todo opt skip rest of bucket if empty
			copy(t_entries[i++],e);
		}
	}

	siz nbucks= heap.size()/DEPTH;
	print(heap.size());
	
	u8 expand_tries= 0;
	//fit depth
	bool done= false;//goto doesnt like raii
	while(!done){//monte carlo
		//grow until <= DEPTH-RESERVE collisions occur on any bucket
		if(expand_tries>4)
			warn("hashmap unreasonable expansion, increase grow factor, decrease pad, or unfuck hash algo");
		expand_tries++;

		nbucks*= grow_factor;
		arr_raii<u8> bcounts(nbucks);
		zero(bcounts);
		done= true;
		EACH(entry,t_entries){
			hash_t h= hash(entry.k);
			u8& c= bcounts[h%nbucks];//bucket's member count
			c++;
			if(c+expand_pad>=DEPTH){//must reserve at least 1
				done= false;
				break;
			}
		}
	}

	realloc(heap,nbucks*DEPTH);
	ass(heap.size()%DEPTH==0);
	EACH(e,heap)
		e.empty= true;

	//repopulate
	EACH(e,t_entries)
		copy(*_alloc(e.k), e.v);//will not recurse
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
	RA(expansion,2){
		idx i= bucket(k)*DEPTH;
		RA(b,DEPTH){
			slot& at= heap[i+b];
			if(at.empty){//empty slot found
				entry_count++;
				at.empty= false;
				at.k= k;
				return &(at.v);
			}
			else if(at.k==k){//entry with key already present
				warn("hmap overwrite");
				return &(at.v);
			}
		}
		//could not fit in bucket
		ass(expansion==0);
		//expansion guarantees >=1 free slot per bucket
		expand();
	}
	unreachable; return null;
}


template<typename K, typename V> 
template<typename... E>
V& hash_map<K,V>::add(K k, E... e){
	//ppack will handle move ctor properly
	//do not confuse c++ template ppack with c variadic function
	return *new(_alloc(k))V(e...);
}

template<typename K, typename V> 
bool hash_map<K,V>::sub(K k){
	idx i= bucket(k)*DEPTH;
	RA(b,DEPTH){
		slot& at= heap[i+b];
		if(at.empty)//empty always procedes unempty
			return false;
		if(at.k==k){//match
			//start at end, walking back until finding
			//a bucket that is non null. may be self
			RD(b2,DEPTH){
				slot& swap= heap[i+b2];//may == at
				if(swap.empty)//end element to be moved
					continue;
				copy(at,swap);
				swap.empty= true;
				entry_count-=1;
				return true;
			}
			//there will not be a scenario where
			//b2<b || b2>depth; as at must be non null
			unreachable;
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
	EACH(s,heap)
		if(!s.empty)
			r+=s.k;
}
template<typename K, typename V> 
void hash_map<K,V>::values_cpy(vector<V>& v){
	EACH(s,heap)
		if(!s.empty)
			v+=s.v;
}

}