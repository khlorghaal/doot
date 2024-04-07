#pragma once
#include "time.hpp"
#include "math.hpp"
#include "vector.hpp"
#include "hash_map.hpp"
#include "idheap.hpp"
#include "ringbuffer.hpp"

namespace doot{
void run_tests(){
	cout("tests");
	profiler pf;
	{
		ass((str("")+="")=="");
		ass(str("asdf")=="asdf");
		ass(str("asdf")!="zxcv");
		str a= "";
		ass(a=="");
		str b= "asdf";
		ass(b=="asdf");
		//str c= b; c+="zxcv";
		//ass(c=="asdfzxcv");
		//str d= c; d.clear(); d+= "qwer";
		//ass(d=="qwer");
		//str g= str::fmt("a%s","b")+strfmt("%s","c");
		//ass(g=="abc");
		str f= str::fmt("%i %i %u %#x %#.2f 0",0,-1,-1,-1,1.f);
		ass(f=="0 -1 4294967295 0xffffffff 1.00 0");
		//str e= d; e+=strfmt("%s%i",(cstr)d,0);
		//ass(e=="qwerqwer0");
	}
	
	{//math
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


		//scalars
		ass(eps( 360_deg, 1_rad *TAU ));
		ass(eps( 360_deg, rad{1.*TAU}));
		ass( 1_s == 1000_ms);
		ass( 1_s == 1000000_us);
		ass( 1_s == 1000000000_ns);
		ass( 1_s == 1000_ms);
		ass( 1_s == 1000000_us);
		ass( 1_s == 1000000000_ns);
		ass( 1000_ms == 1000000_us );
		ass( 1_s + 1000_ms == 2_s);
		ass( 1_s + 1000_ms == 2_s);
		ass(eps(1_s * 2.f, 2_s));
	}
	
	{//rng
		{//global
			float racc= 0;
			RA(i,0x1000)
				racc+= sfrand();
			racc/=0x1000;
			ass(abs(racc)<.01);
		}
		{//average
			i64 racc= 0;
			RA(i,0x1000){//
				racc+= (i32)hash((u64)i);
				racc+= (i32)hash((u32)i);
			}
			ass( abs(racc/0x1000ll) < INTMAX<hash_t> );
		}
		{//modular
			//i64 a= 0;
			//RA(k,0x10){
			//RA(j,0x10){
			//	RA(i,0x100){
			//		a+= (hash(i*j*k)%(k+2));
			//	}
			//	//print(a/(k+2));
			//	a=0;
			//}}
		}
	}
	
	{//nxpo2
		ass(nxpo2(0)==1);
		ass(nxpo2(1)==2);
		ass(nxpo2(2)==4);
		ass(nxpo2(3)==4);
		ass(nxpo2(0x01000000)==0x02000000);
		ass(nxpo2(0x01000001)==0x02000000);
		ass(nxpo2(0x0fffffff)==0x10000000);
	}
	
	{//vector
		vector<int> vec(1);
		RA(i,512)
			vec.add(i);
		RA(i,512){
			ass(vec[i]==i);
			ass(vec.ptr_idx(vec.base+i)==i);
		}
		vector<int> vecb;
		vecb.addv(vec);
		RA(i,512)
			ass(vec.sub_eq(i));
		RD(i,512)
			ass(vecb.sub_eq(i));
	}
	
	{//hashmap
		hash_map<int, int> map;
		RA(i,512){
			print(i);
			map.add(i,i);
		}
		RA(i,512){
			int* p= map[i];
			ass(!!p);
			ass(*p==i);
		}
	}
	
	{//idheap
		idheap<int> heap;
		RA(i,512)
			heap.add(i,i);
		RA(i,512){
			auto& e= *heap[i];
			ass(e==i);
			ass(heap.ptr_id(&e)==i);
			ass(heap.index(i)==i);
		}
		RA(i,512)
			heap.sub(i);
		RA(i,512)
			ass(heap.map[i]==NULLIDX);
		RD(i,511)
			heap.add(i,0);//??? what?
	}

	
	{//kitchen sink container
		idheap<hash_map<str,vector<str>>> wew;
		hash_map<str,vector<str>>& a= wew.add(0);
		vector<str>& b= a.add("a");
		str& c= b.add("b");
		ass((*(*wew[0])["a"])[0]=="b");
	}

	
	{//ringbuffer
		ringbuffer<int, 512> rb;
		RA(i,512)
			rb<<i;
		RA(i,512)
			ass(rb[i]==i);	
		RA2(i,512,1024)
			rb<<i;
		for(int i=512; i!= 1024; i++)
			ass(rb[i]==i);
	}
	/*
	
	{//

	}
	*/
	cout("tests completed");
};
}
