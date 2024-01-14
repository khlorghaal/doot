#pragma once
#include "doot.hpp"
#include "string.hpp"

namespace doot{

extern ms current_time_ms();//os.cpp
extern us current_time_us();
extern void sleep(us);

class Timer{
	ms tInvoke, tBegin, tBeginp;

	int targetTimeus;

	volatile bool waiting= false;

	float spf_avg;//lerped average

public:
	ms total_time=0;//ms since start
	u64 tick=0;//number of invocations
	float spf=0;//seconds per frame

	Timer(float fps);

	/**@param cap -1 if unlimited*/
	void setMaxUpdatesPerSecond(float cap);

	void invoke();

	void pause();
	void resume();

	float getAverageSpf();
	int getAverageFps();
	/**thisrate * k = thatrate*/
	float getRelativeRate(Timer& that);
};

struct profiler{
	str name;
	us beg;
	us end;

	void start(str);
	void stop();
};

}
