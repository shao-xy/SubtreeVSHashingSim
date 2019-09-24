#include "include/LRU.h"
#include "common/Debug.h"

int main()
{
	LRUCache<int, int> cache(2);
	cache.set(1, 2);
	cache.set(3, 4);
	cache.set(5, 6);

	dout << cache.get(3) << dendl;
	
	cache.set(7, 8);

	dout << cache.get(3) << dendl;

	return 0;
}
