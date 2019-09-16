#ifndef __COMMON_LRU_H_
#define __COMMON_LRU_H_

#include <list>
#include <unordered_map>

template <typename KT, typename VT>
class LRUCache {
private:
	struct Node {
		KT key;
		VT value;
		Node(KT k, VT v) : key(k), value(v) {}
	};
public:
	LRUCache(int c) : capacity(c) {}
	
	VT get(KT key) {
		if (cacheMap.find(key) == cacheMap.end())
			return NULL; // TODO: Here we assume VT as pointer type
		cacheList.splice(cacheList.begin(),cacheList,cacheMap[key]);
		cacheMap[key] = cacheList.begin();
		return cacheMap[key]->value;
	}
	void set(KT key, VT value) {
		if (cacheMap.find(key) == cacheMap.end())
		{
			if (cacheList.size() == capacity)
			{
				cacheMap.erase(cacheList.back().key);
				cacheList.pop_back();
			}
			cacheList.push_front(Node(key,value));
			cacheMap[key] = cacheList.begin();
		} 
		else
		{
			cacheMap[key]->value = value;
			cacheList.splice(cacheList.begin(),cacheList,cacheMap[key]);
			cacheMap[key] = cacheList.begin();
		}
	}
private:
	int capacity;

	std::list<Node> cacheList;
	std::unordered_map<KT, typename std::list<Node>::iterator> cacheMap;
};

#endif /* common/LRU.h */
