#include "thread.hpp"
namespace doot{

//threadsafe queue
tplt class quem{
	mutex mut;
	cons<T*>* car;
public:
	void push(T* e){
		if(!e){
			bad("quem::push( NULL )");
			re;
		}
		auto* n= new cons<T*>{e,{null}};
		mut.lock();
		if(auto* t= tail(car))
		   t->b= n;
		else
		   car= n;
		mut.locknt();
	}
	maybe<T*> pull(){
		mut.lock();
		if(!car){
			mut.locknt();
			nope;
		}
		 may<T*>  a= car->a;
		cons<T*>* b= car->b;
		delete car;
		car= b;

		mut.locknt();
		re a;
	}
	bool peek(){//1+
		bool r;
		mut.lock();
		r= !!car;
		mut.locknt();
		retr;
	}
	bool peek_next(){//2+
		bool r;
		mut.lock();
		r= car && car->b;
		mut.locknt();
		retr;
	}

	maybe<T*> dump(){//discards all and returns last
		mut.lock();
		if(!car){
			mut.locknt();
			nope;
		}
		while(ato* n= car->b){//next
			delete car->a;
			delete car;
			car= n;
		}
		T* r= car->a;//disown
		ass(r);
		delete car;
		car= null;
		mut.locknt();
		retr;
	}
};

}