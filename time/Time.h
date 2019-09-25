#ifndef __TIME_ETERNALTIME_H__
#define __TIME_ETERNALTIME_H__

#include "include/Random.h"

typedef unsigned int Time;

class WindingTime {
protected:
	Time _time;
	RandomGenerator _rg;
public:
	WindingTime(Time t = 0) : _time(t) {}

	virtual void tick(Time some_time);
	void tick_random(Time lower_bound = 0, Time upper_bound = 0);
	void tick_random(RandomRange r);
};

class EternalTime : public WindingTime {
public:
	EternalTime() {}

	Time gettimeofday() { return _time; }
	void tick(Time some_time) override { _time += some_time; }
};

class StopWatch : public WindingTime {
public:
	StopWatch() {}

	Time get() { return _time; }
	void set(Time t) { _time = t; }
	void reset_zero() { set(0); }
};

extern EternalTime get;
extern StopWatch gsw;

#endif /* time/EternalTime.h */
