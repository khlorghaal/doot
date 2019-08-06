#include "timer.hpp"
#include "math.hpp"
#include "vector.hpp"
#include "hash_map.hpp"
#include "mapped_heap.hpp"
#include "ringbuffer.hpp"

namespace doot{
void runTests(){
	profiler pf;
	pf.start("math");
	{
		assert( min(0,1)==0 );
		assert( max(0,1)==1 );

		assert( clamp( 0., 0.,1.)==0. );
		assert( clamp(-1., 0.,1.)==0. );
		assert( clamp( 2., 0.,1.)==1. );

		assert( lerp(.5, 0., 1.)== .5 );
		assert( lerp(.5, 1., 2.)==1.5 );
		assert( lerp(0, 1., 20000.)==1. );
		assert( lerp(10000., 0.,0.)==0. );

		#define angneq(a,b) assert( fabs(angn((a))-(b)) < 1.e-3 )
		angneq( PI/3+TAU,  PI/3);
		angneq( PI/3-TAU,  PI/3);
		angneq(-PI/3+TAU, -PI/3);
		angneq(-PI/3-TAU, -PI/3);

		#define dangeq(a,b,c) assert( angn( angn((b)-(a))-(c) )<1.e-4f );
		dangeq( 0, 0, 0);
		dangeq( 1, 1, 0);
		dangeq( 0, 1, 1);
		dangeq( 1, 0,-1);
		dangeq( 0,-1,-1);
		dangeq(-1, 0, 1);
		dangeq(-1,-1, 0);
		dangeq( PI, PI,0);
		dangeq( PI,-PI,0);
		dangeq(-PI, PI,0);
		dangeq(-PI,-PI,0);
		dangeq( TAU, TAU,0);
		dangeq( TAU,-TAU,0);
		dangeq(-TAU, TAU,0);
		dangeq(-TAU,-TAU,0);
		dangeq( TAU,0,0);
		dangeq(-TAU,0,0);
		dangeq(0, TAU,0);
		dangeq(0,-TAU,0);
		dangeq(TAU,2*TAU,0);
		dangeq(2*TAU,TAU,0);
		dangeq( PI,TAU,PI);
		dangeq(-PI,TAU,PI);
		dangeq( PI/2,0,-PI/2);
		dangeq(-PI/2,0, PI/2);
		dangeq( PI/2,-PI/2, PI );
		dangeq(-PI/2, PI/2, PI );
		dangeq( PI, PI/2,-PI/2 );
		dangeq(-PI, PI/2,-PI/2 );
		dangeq( PI,-PI/2, PI/2 );
		dangeq(-PI,-PI/2, PI/2 );
		dangeq(TAU+PI/2,     PI, PI/2);
		dangeq(    PI/2, TAU+PI, PI/2);
		dangeq(TAU+ PI/2,    -PI/2, PI );
		dangeq(    -PI/2, TAU+PI/2, PI );
		dangeq(     PI/2, TAU-PI/2, PI );
		dangeq(TAU+-PI/2, TAU+PI/2, PI );
		dangeq( PI*3/4, -PI*3/4,  PI/2);
		dangeq(-PI*3/4,  PI*3/4, -PI/2);

		assert(hash(    0)==hash(    0));
		assert(hash(0x100)==hash(0x100));
		
	}
	pf.stop();
	pf.start("rng");
	{
		float racc= 0;
		forcount(i,1024)
			racc+= sfrand();
		racc/=1024;
		assert(abs(racc)<.01);
	}
	pf.stop();
	pf.start("vector");
	{
		vector<int> vec(1);
		forcount(i,512)
			vec<<i;
		forcount(i,512){
			assert(vec[i]==i);
			assert(vec.ptr_idx(vec.base+i)==i);
		}
		forcountdown(i,511)
			assert(vec.remove_eq(i));
	}
	pf.stop();
	pf.start("hashmap");
	{
		hash_map<int, int> map;
		forcount(i,512)
			*map.put(i)= i;
		forcount(i,512)
			assert(*map[i]==i);
	}
	pf.stop();
	pf.start("mappedheap");
	{
		mapped_heap<int> heap;
		forcount(i,512)
			heap.put(i,i);
		forcount(i,512){
			auto& e= *heap[i];
			assert(e==i);
			assert(heap.ptr_id(&e)==i);
			assert(heap.index(i)==i);
		}
		forcount(i,512)
			heap.kill(i);
		forcount(i,512)
			assert(heap.map[i]==NULLIDX);
		forcountdown(i,511)
			heap.put(i,0);//???
	}
	pf.stop();
	/*
	pf.start("multimappedheap");
	{
		multimapped_heap<int,8> heap;
		vector<triad<eid,cid,int>> vec;
		forcount(e,512){
			forcount(i,8){
				cid c;
				int val= e+i;
				heap.make(e,c)= val;
				vec.push({(eid)e,c,val});
			}
		}
		assert(heap.cidget.next==512*8);
		dynarr<int,8> group;
		for(auto& v : vec){
			assert(heap.cid2eid(v.b)==v.a);
			assert(*heap.cid2t(v.b)==v.c);
			assert(find(heap.eid2cids(v.a), v.b)!=NULLID);

			for(int* vp : heap[v.a])
				group.push(*vp);
			assert(find((arr<int>)group,v.c)!=NULLID);
			group.clear();
		}
		for(auto& v: vec)
			heap.kill(v.b);
	}
	pf.stop();
	*/
	pf.start("ringbuffer");
	{
		ringbuffer<int, 512> rb;
		forcount(i,1024)
			rb<<i;
		for(int i=512; i!= 1024; i++)
			assert(rb[i]==i);
	}
	pf.stop();
	/*
	pf.start("");
	{

	}
	pf.stop();
	*/
	cnsl<<"tests completed"<<endl;
};
}