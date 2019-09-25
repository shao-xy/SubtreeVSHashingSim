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
