/* timer.cpp
 *
 *  Created on: Oct 14, 2015
 *  Author: Khlorghaal
 *	All Rights Reserved.
 *	Do not distribute.
 */

#include "timer.hpp"
#include <chrono>
#include <thread>
#include <iostream>
using namespace std::this_thread;
using namespace std::chrono;

namespace doot{

Timer::Timer(float fps){
	setMaxUpdatesPerSecond(fps);
	tInvoke= tBegin= tBeginp= current_time();
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
	tInvoke= current_time();
	ms tProcess= tInvoke-tBegin;
	ms dt= tBegin-tBeginp;
	spf= (dt+1e-16f)/1000.f;

	spf_avg= spf_avg*.8 + spf*.2;

	i64 wait= targetTimeus-tProcess*1000;
	waiting= true;
	if(wait>250)
		sleep_for(microseconds(wait));
	else
		;
	waiting= false;

	tBeginp= tBegin;
	tBegin= current_time();
	total_time+= tBegin-tBeginp;
	tick++;
}


/**thisrate * k = thatrate*/
float Timer::getRelativeRate(Timer& that){
	return that.spf/spf;
}


void profiler::start(str const& name_){
	name= name_;
	beg= current_time();
};
void profiler::stop(){
	end= current_time();
	cout(str::fmt("profiler: %20s: %5lli ms", name.cstr(), end-beg));
};

}