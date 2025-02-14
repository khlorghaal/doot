#pragma once
#include "math.hpp"
#include "2d.hpp"
#include "time.hpp"
#include "list.hpp"
#include "hmap.hpp"
#include "idheap.hpp"
#include "ringbuffer.hpp"
#include "algos.hpp"
#include "thread.hpp"

namespace doot{

void run_tests(){
	cout("doot::tests");
	profiler pf;
	{
		ass(sizeof(int )==4);
		ass(sizeof(long)==8);
		ass(sizeof(long int )==8);
		ass(sizeof(long long)==8);
		ass(sizeof(long long int)==8);

		ass(sizeof(i8 )==1);
		ass(sizeof(i16)==2);
		ass(sizeof(i32)==4);
		ass(sizeof(i64)==8);
		ass(sizeof(u8 )==1);
		ass(sizeof(u16)==2);
		ass(sizeof(u32)==4);
		ass(sizeof(u64)==8);

		ass(sizeof(' ' )==1);
		ass(sizeof(0   )==4);
		ass(sizeof(0l  )==8);
		ass(sizeof(0ll )==8);
		ass(sizeof(0u  )==4);
		ass(sizeof(0ul )==8);
		ass(sizeof(0ull)==8);
	}
	{
		{ may<u8>  a;    ass(!a); }
		{ may<id>  a;    ass(!a); }
		{ may<u8>  a={}; ass(!a); }
		{ may<id>  a={}; ass(!a); }
	}
	{
		ass((str("")+="")=="");
		ass((str(str(""))+="")=="");
		ass((str(str(""))+=str(""))==str(str("")));
		ass(str("asdf")=="asdf");
		ass(str("asdf")!="asd");
		ass(str("asdf")!="asdfb");
		ass(str("asdf")!="zxcv");
		ass(str()+(0   )=="0");
		ass(str()+(0l  )=="0");
		ass(str()+(0ll )=="0");
		ass(str()+(0u  )=="0");
		ass(str()+(0ul )=="0");
		ass(str()+(0ull)=="0");

		ass(str()+(0.1f )=="0.1000");
		ass(str()+(0.1  )=="0.1000");
		ass(str()+(0   )=="0");
		str a= "" ;{ str a( "");
		ass(a=="");} ass(a==""); //khlor is struck with a possessed mood
		ass(!a);
		a.cat(0); a.cat(str(0));//make sure not converted to cstr
		ass(a=="00");
		str b= "asdf";
		ass(b=="asdf");
		str c= b; c+="zxcv";
		ass(c=="asdfzxcv");
		c.cat("vcxz");
		ass(c=="asdfzxcvvcxz");
		str d= c; d.clear(); d+= "qwer";
		ass(d=="qwer");
		str e= d;
		ass(e==d);
		e+=str(d)+0;
		e.cat(0);
		ass(e=="qwerqwer00");
		str g= str("a")+"b"+"c";
		ass(g=="abc");
		//str f= str::fmt("%i %i %u %#x %#.2f 0",0,-1,-1,-1,-1,1.f);
		//ass(f=="0 -1 4294967295 0xffffffff 1.00 0");
		str i;
		i+= 1;i+= 1.; i+= ""_s+1+1ll+1u+1ull;
		ass(i=="11.00001111");
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
			ra(i,0x1000)
				racc+= sfrand();
			racc/=0x1000;
			ass(abs(racc)<.01);
		}
		{//average
			i64 racc= 0;
			ra2(i,-0x1000,0x1000){//
				racc+= (i32)hash((i64)i);
				racc+= (i32)hash((i32)i);
			}
			ass( abs(racc/0x2000ll) < INTMAX<hash_t> );
			//fixme lbtm
		}
		{//modular
			//i64 a= 0;
			//ra(k,0x10){
			//ra(j,0x10){
			//	ra(i,0x100){
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
	
	{//list
		list<int> vec(1);
		ra(i,512)
			vec.add(i);
		ra(i,512){
			ass(vec[i]==i);
			ass(vec.ptr_idx(vec.base+i)==i);
		}
		list<int> vecb;
		vecb.addl(vec);
		ra(i,512)
			ass(vec.sub_eq(i));
		rd(i,512)
			ass(vecb.sub_eq(i));

		list<u16> b;
		b.realloc(256);
		ass(b.size()==0);
		ass(b.capacity()==256);
		b.base[255]=0;
		b.prealloc(512);
		b.base[511]=0;
		ass(b.capacity()>=512);

		list<u8> m(1);
		m.realloc(1);
		m.prealloc(8);
		m.base[7]=0;

		list<u8> m2= list<u8>(list<u8>(1));
		m2.realloc(1);
		m2.prealloc(8);
		m2.base[7]=0;		
	}

	{//algos
		lis<u32> l;
		ra(i,5)
			l+=i;
		ass( sum(l)==10 );
		ass( average(l)==2 );

		lis<arr<u32>> r;
		lis<u32> t0; t0+= 0; t0+= 1;
		lis<u32> t1; t1+= 2; t1+= 3; t1+= 4;
		div(l,2,r);
		ass(r.size()==2);
		ass(r[0]==t0);
		ass(r[1]==t1);
	}
	
	{//hashmap
		hmap<i32, i32> map;
		ra2(i,-0x100,0x100){
			map.add(i,i);
		}
		hmap<i64, i64> map2;
		ra2(i,-0x100,0x100){
			map2.add(i,i);
		}
		ra(i,0x100){
			may<int> p= map[i];
			ass(!!p);
			ass(p.un()==i);
		}
	}
	
	{//idheap
		idheap<i32> heap;
		ra(i,512)
			ass(!heap.index(i));
		ra(i,512)
			heap.add(i,i);
		ra(i,512){
			i32& e= heap[i].un();
			ass(e==i);
			//ass(heap.ptr_id(&e)==i);
			ass(heap.index(i).un()==i);
		}
		ra(i,512){
			ass(!!heap[i]);
			ass(!!heap.index(i));
			heap.sub(i);
		}
		ra(i,512)
			ass(!heap.index(i));
		rd(i,512)
			heap.add(i,0);//check still intact

		ass(!heap[-1]);
		ass(!heap.index(-1));
	}

	{//kitchen sink container
		idheap<hmap<str,list<str>>> wew;

		       hmap<str,list<str>>& a= wew.add(0);
		           ass(wew[0].t == &a);
		                 list<str>& b=   a.add("a");
		           ass(a["a"].t == &b);
		                       str& c=   b.add("b");
		           ass(&b[0]    == &c);
		//these dont invalidate refs as additions are subobjects
		ass( wew[0].un()["a"].un()[0][0] =='b');
		       hmap<str,list<str>>& a_= wew[0].un();
		                list<str>&  b_= a_["a"].un();
		                     str&   c_= b_[0];
		ass(c_[0]=='b');

		wew.add(1).add("z").add("y");
		ass( wew[1].un()["z"].un()[0][0] =='y');
	}
	
	{//ringbuffer
		ringbuffer<int, 512> rb;
		ra(i,512)
			rb<<i;
		ra(i,512)
			ass(rb[i]==i);	
		ra2(i,512,1024)
			rb<<i;
		ra2(i,512,1024)
			ass(rb[i]==i);
	}

	{//thread
		int i0= 0;
		thread("test call_t; should die",{[](void* v){
			(*(int*)v)++;
		},&i0});

		mutex mut0;
		mut0.lock();
		thread("test mut0; should die",{[](void* v){
			are mut0= vcas<mutex>(v);
			mut0.lock();//wait
			mut0.locknt();
		},&mut0});
		mut0.locknt();

		lock lock0;
		thread("test lock0; should die",{[](void* v){
			are lock0= vcas<lock>(v);
			lock0.wait();
		},&lock0});
		lock0.wake(true);

		latch latch0;
		latch0.set(8);
		thread("test latch0; should die",{[](void* v){
			are latch0= vcas<latch>(v);
			ra(i,8)
				latch0.tick();
		},&latch0});
		latch0.wait();

		arr_raii<u32> a(0x1000);
		each(o,a) o= 69;
		list<u32> b;
		warp::dispatch<u32,u32,[](arr<u32>& a, list<u32>& b){
				each(o,a)
					o= 1;
				b+= sum(a);
			}>(a,b);
		u32 s= sum(b);
		ass(s==0x1000);
	}
	/*
	
	{//

	}
	*/
	cout("doot:: tests pass");
};
};
