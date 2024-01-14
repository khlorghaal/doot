/* timer.cpp
 *
 *  Created on: Oct 14, 2015
 *  Author: Khlorghaal
 *	All Rights Reserved.
 *	Do not distribute.
 */

#include "timer.hpp"

namespace doot{

Timer::Timer(float fps){
	setMaxUpdatesPerSecond(fps);
	tInvoke= tBegin= tBeginp= current_time_ms();
}

void Timer::setMaxUpdatesPerSecond(float cap){
	if(cap==-1 || cap==0)
		targetTimeus= 0;
	targetTimeus= int(1000000.f/cap);
}

void Timer::invoke(){
	/*
	|__________|________|
	   process    wait
	^tbegp     ^tinvk   ^tbeg
	*/
	tInvoke= current_time_ms();
	ms tProcess= tInvoke-tBegin;
	ms dt= tBegin-tBeginp;
	spf= (dt+1e-16f)/1000.f;

	spf_avg= spf_avg*.8 + spf*.2;

	us wait= targetTimeus-tProcess*1000;
	waiting= true;
	if(wait>250)
		sleep(wait);
	else
		;
	waiting= false;

	tBeginp= tBegin;
	tBegin= current_time_ms();
	total_time+= tBegin-tBeginp;
	tick++;
}


/**thisrate * k = thatrate*/
float Timer::getRelativeRate(Timer& that){
	return that.spf/spf;
}


void profiler::start(str name_){
	name= name_;
	beg= current_time_ms();
};
void profiler::stop(){
	end= current_time_ms();
	cout(strfmt("profiler: %20s: %5lli ms", (cstr)name, end-beg));
};

}