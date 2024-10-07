#include "thread.hpp"
namespace doot{

//threadsafe queue
tplt class quem{
	mutex mut;
	cons<T>* car;
public:
	void push(T* e){
		ass(!!e);
		auto* n= new cons<T>{e,{null}};
		mut.lock();
		if(auto* t= tail(car))
		   t->b= n;
		else
		   car= n;
		mut.locknt();
	}
	maybe<T> pull(){
		mut.lock();
		if(!car){
			mut.locknt();
			nope;
		}
		 may<T>  a= car->a;
		cons<T>* b= car->b;
		delete car;
		car= b;

		mut.locknt();
		re a;
	}
};

}