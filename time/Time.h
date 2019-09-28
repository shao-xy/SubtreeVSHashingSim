#ifndef __TIME_ETERNALTIME_H__
#define __TIME_ETERNALTIME_H__

#include <string>
using std::string;

#include "include/Random.h"

typedef unsigned int TimeVal;

struct Time {
	TimeVal us;
	TimeVal ns;

	Time(TimeVal us = 0, TimeVal ns = 0) : us(us), ns(ns) {}
	Time(const Time & another) : us(another.us), ns(another.ns) {}

	operator double() const {
		return (double)us + ((double)ns / 1000);
	}
};

inline Time operator+(const Time & l, const Time & r)
{
	return Time(l.us + r.us + (l.ns + r.ns) / 1000, (l.ns + r.ns) % 1000);
}

inline Time & operator+=(Time & l, const Time & r)
{
	l.us += r.us;
	l.ns += r.ns;
	l.us += l.ns / 1000;
	l.ns = l.ns % 1000;
	return l;
}

class WindingTime {
protected:
	Time _time;
	RandomGenerator _rg;
public:
	WindingTime(TimeVal us = 0) : _time(us, 0) {}
	WindingTime(Time t) : _time(t) {}

	virtual void tick(Time some_time);
	void tick_random(Time lower_bound = 0, Time upper_bound = 0);
	void tick_random(RandomRange r);
	void tick_random(string type, size_t repeat_time = 1);
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
	void set(TimeVal us) { _time.us = us; }
	void set(Time t) { _time = t; }
	void reset_zero() { set(0); }
};

extern EternalTime get;
extern StopWatch gsw;

#endif /* time/EternalTime.h */
