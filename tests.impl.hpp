#pragma once
#include "timer.hpp"
#include "math.hpp"
#include "vector.hpp"
#include "hash_map.hpp"
#include "idheap.hpp"
#include "ringbuffer.hpp"

#ifdef DEBUG
namespace doot{
void run_tests(){
	profiler pf;
	{
		ass((str("")+="")=="");
		ass(str("asdf")=="asdf");
		ass(str("asdf")!="zxcv");
		str a= "";
		ass(a=="");
		str b= "asdf";
		ass(b=="asdf");
		str c= b; c+="zxcv";
		ass(c=="asdfzxcv");
		str d= c; d.clear(); d+= "qwer";
		ass(d=="qwer");
		str g= str::fmt("a%s","b").fmtcat("%s","c");
		ass(g=="abc");
		str f= str::fmt("%i %i %u %#x %#.2f 0",0,-1,-1,-1,1.f);
		ass(f=="0 -1 4294967295 0xffffffff 1.00 0");
		str e= d; e.fmtcat("%s%i",d.cstr(),0);
		ass(e=="qwerqwer0");
	}

	pf.start("math");
	{
		ass( min(0,1)==0 );
		ass( max(0,1)==1 );

		ass( clamp( 0., 0.,1.)==0. );
		ass( clamp(-1., 0.,1.)==0. );
		ass( clamp( 2., 0.,1.)==1. );

		ass( lerp(.5, 0., 1.)== .5 );
		ass( lerp(.5, 1., 2.)==1.5 );
		ass( lerp(0, 1., 20000.)==1. );
		ass( lerp(10000., 0.,0.)==0. );

		#define angneq(a,b) ass( fabs(angn((a))-(b)) < 1.e-3 )
		angneq( PI/3+TAU,  PI/3);
		angneq( PI/3-TAU,  PI/3);
		angneq(-PI/3+TAU, -PI/3);
		angneq(-PI/3-TAU, -PI/3);

		#define dangeq(a,b,c) ass( angn( angn((b)-(a))-(c) )<1.e-4f );
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

		ass(hash(    0)==hash(    0));
		ass(hash(0x100)==hash(0x100));
		ass(hash(-1)==hash(-1));
		
	}
	pf.stop();
	pf.start("rng");
	{
		float racc= 0;
		forcount(i,1024)
			racc+= sfrand();
		racc/=1024;
		ass(abs(racc)<.01);
	}
	pf.stop();
	pf.start("vector");
	{
		vector<int> vec(1);
		forcount(i,512)
			vec.make(i);
		forcount(i,512){
			ass(vec[i]==i);
			ass(vec.ptr_idx(vec.base+i)==i);
		}
		vector<int> vecb;
		vecb.make(vec);
		forcount(i,512)
			ass(vec.remove_eq(i));
		forcountdown(i,512)
			ass(vecb.remove_eq(i));
	}
	pf.stop();
	pf.start("hashmap");
	{
		hash_map<int, int> map;
		forcount(i,512)
			map.make(i,i);
		forcount(i,512){
			int* p= map[i];
			ass(!!p);
			ass(*p==i);
		}
	}
	pf.stop();
	pf.start("mappedheap");
	{
		idheap<int> heap;
		forcount(i,512)
			heap.make(i,i);
		forcount(i,512){
			auto& e= *heap[i];
			ass(e==i);
			ass(heap.ptr_id(&e)==i);
			ass(heap.index(i)==i);
		}
		forcount(i,512)
			heap.kill(i);
		forcount(i,512)
			ass(heap.map[i]==NULLIDX);
		forcountdown(i,511)
			heap.make(i,0);//???
	}
	pf.stop();

	pf.start("kitchen sink container");
	{
		idheap<hash_map<str,vector<str>>> wew;
		auto& a= wew.make(0);
		auto& b= a.make("a");
		auto& c= b.make("b");
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
				vec.make({(eid)e,c,val});
			}
		}
		ass(heap.cidget.next==512*8);
		dynarr<int,8> group;
		for(auto& v : vec){
			ass(heap.cid2eid(v.b)==v.a);
			ass(*heap.cid2t(v.b)==v.c);
			ass(find(heap.eid2cids(v.a), v.b)!=NULLID);

			for(int* vp : heap[v.a])
				group.make(*vp);
			ass(find((arr<int>)group,v.c)!=NULLID);
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
			ass(rb[i]==i);
	}
	pf.stop();
	/*
	pf.start("");
	{

	}
	pf.stop();
	*/
	cout("tests completed");
};
}
#endif