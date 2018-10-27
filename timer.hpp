/* timer.hpp
 *
 *  Created on: Oct 14, 2015
 *  Author: Khlorghaal
 *	All Rights Reserved.
 *	Do not distribute.
 */

#pragma once

#include "global.hpp"
#include "ringbuffer.hpp"

namespace doot{

typedef time_t int64;//ms
time_t current_time();//os.cpp
int64 current_time_us();

class Timer{
	time_t tInvoke, tBegin, tBeginp;

	int targetTimeus;

	volatile bool waiting= false;

	ringbuffer<float, 60> prevspf;

public:
	time_t total_time=0;//ms since start
	uint64 tick=0;//number of invocations
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
	string name;
	time_t beg;
	time_t end;

	void start(string);
	void stop();
};

}
