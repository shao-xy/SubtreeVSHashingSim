#include "Time.h"

EternalTime get;
StopWatch gsw;

void WindingTime::tick(Time some_time)
{
	_time += some_time;
	get.tick(some_time);
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
			tick_random(25, 35);
		else if (type == "memory")
			tick_random(0, 1);
		else if (type == "ipc")
			tick_random(6, 8);
		else if (type == "IPoIB")
			tick_random(15, 20);
		else
			throw "Time: Not supported type of communication delay";
	}
}
