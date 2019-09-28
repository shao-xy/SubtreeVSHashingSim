#include "Time.h"

#include "common/Debug.h"

EternalTime get;
StopWatch gsw;

ostream & operator<<(ostream & os, Time t)
{
	os << "[Time " << (double)(t) << "ms]";
	return os;
}

void WindingTime::tick(Time some_time)
{
	_time += some_time;
	get.tick(some_time);
	if (this == &gsw)
		cout << " Tick: " << some_time << dendl;
}

void WindingTime::tick_random(Time lower_bound, Time upper_bound)
{
	tick_random(RandomRange(lower_bound, upper_bound));
}

void WindingTime::tick_random(RandomRange r)
{
	tick(_rg(r));
}

void WindingTime::tick_random(string type, size_t repeat_time)
{
	for (size_t i = 0; i < repeat_time; i++) {
		if (type == "tcp")
			tick_random(30, 40);
		else if (type == "memory")
			tick_random(Time(0, 50), Time(0, 150));
		else if (type == "ipc")
			tick_random(6, 8);
		else if (type == "IPoIB")
			tick_random(20, 25);
		else
			throw "Time: Not supported type of communication delay";
	}
}
