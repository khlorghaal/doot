#pragma once
#include "doot.hpp"
#include "string.hpp"

namespace doot{

inl nsec op""_ns(u64 v){ re v; }
inl nsec op""_us(u64 v){ re v*1000; }
inl nsec op""_ms(u64 v){ re v*1000000; }
inl nsec op""_s( u64 v){ re v*1000000000; }
inl time_t usec(nsec t){ re t/1000; }
inl time_t msec(nsec t){ re t/1000000; }
inl time_t  sec(nsec t){ re t/1000000000; }


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

}
