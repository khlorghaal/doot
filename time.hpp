#pragma once
#include "doot.hpp"
#include "string.hpp"

namespace doot{

extern nsec current_time();//os.cpp
extern void sleep(nsec);

class Timer{
	nsec tInvoke, tBegin, tBeginp;
	nsec targetTime;

	volatile bool waiting= false;

	f32 spf_avg;//lerped average

public:
	nsec total_time=0_ns;//ms since start
	u64 tick=0;//number of invocations
	f32 spf=0;//seconds per frame

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
