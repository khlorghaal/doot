#include "time.hpp"

namespace doot{

Timer::Timer(f32 fps){
	setMaxUpdatesPerSecond(fps);
	tInvoke= tBegin= tBeginp= current_time();
}

void Timer::setMaxUpdatesPerSecond(f32 cap){
	if(cap==-1 || cap==0)
		targetTime= 0_ns;
	targetTime= sec(1)/cap;
}

void Timer::invoke(){
	/*
	|__________|________|
	   process    wait
	^tbegp     ^tinvk   ^tbeg
	*/
	tInvoke= current_time();
	nsec tProcess= tInvoke-tBegin;
	nsec dt= tBegin-tBeginp;
	spf= 1.f/sec(dt);

	spf_avg= spf_avg*.8 + spf*.2;

	nsec wait= targetTime-tProcess;
	waiting= true;
	if(wait>200_us)
		sleep(wait);
	else
		;
	waiting= false;

	tBeginp= tBegin;
	tBegin= current_time();
	total_time+= tBegin-tBeginp;
	tick++;
}


/**thisrate * k = thatrate*/
f32 Timer::getRelativeRate(Timer& that){
	return that.spf/spf;
}


void profiler::start(str name_){
	name= name_;
	beg= current_time();
};
void profiler::stop(){
	end= current_time();
cout(str("profiler: ")+name+": "+msec(end-beg)+" ms");
	//"profiler: %20s: %5lli ms", (cstr)name, msec(end-beg)));
};

};