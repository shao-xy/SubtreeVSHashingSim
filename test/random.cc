#include "include/Random.h"

#include "common/Debug.h"

int main()
{
	RandomGenerator r;
	RandomRange range(0, 1);
	for (int i = 0; i < 100; i++)
	{
		cout << r(range) << dendl;
	}
	return 0;
}
