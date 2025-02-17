#pragma once
#include "doot.hpp"
#include "string.hpp"

namespace doot{

extern nsec current_time();//os.cpp
extern void sleep(nsec);

class Timer{
	nsec tInvoke=0, tBegin=0, tBeginp=0;
	nsec targetTime=0;

	volatile bool waiting= false;

	f32 spf_avg=60.;//lerped average

public:
	nsec total_time=0_ns;//ms since start
	u64 tick=0;//number of invocations
	f32 spf=60.;//seconds per frame since last

	Timer(f32 fps);

	/**@param cap -1 if unlimited*/
	void setMaxUpdatesPerSecond(f32 cap);

	void invoke();

	void pause();
	void resume();

	f32 getAverageSpf();
	f32 getAverageFps();
	/**thisrate * k = thatrate*/
	f32 getRelativeRate(Timer& that);
};

struct profiler{
	str name;
	nsec beg;
	nsec end;

	void start(str);
	void stop();
};

};
