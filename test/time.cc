#include "time/Time.h"

#include "common/Debug.h"

int main()
{
	dout << get.gettimeofday() << dendl;
	get.tick(20);
	dout << get.gettimeofday() << dendl;
	get.tick_random(0, 100);
	dout << get.gettimeofday() << dendl;
}
