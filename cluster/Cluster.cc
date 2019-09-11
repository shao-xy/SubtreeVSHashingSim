#include <algorithm>

#include "Cluster.h"

Cluster::~Cluster() {
	for (Host * host : hosts) {
		delete host;
	}
}
	
Host * Cluster::add_host(string name)
{
	Host * new_host = new Host(this, name);
	hosts.push_back(new_host);
	return new_host;
}

bool Cluster::register_monitor(NetworkEntity * newEntity)
{
	std::vector<NetworkEntity *>::iterator it_m = std::find_if(monitors.begin(), monitors.end(), [=] (NetworkEntity * entity) { return (*entity) == (*newEntity); });
	if (it_m == monitors.end()) {
		monitors.push_back(newEntity);
		return true;
	}
	else
		return false;
}

bool Cluster::unregister_monitor(NetworkEntity * monEntity)
{
	std::vector<NetworkEntity *>::iterator it_m = std::find_if(monitors.begin(), monitors.end(), [=] (NetworkEntity * entity) { return (*entity) == (*monEntity); });
	if (it_m == monitors.end())
		return false;
	else {
		monitors.erase(it_m);
		delete (*it_m);
		return true;
	}
}

NetworkEntity * Cluster::get_monitor()
{
	if (monitors.size() == 0)	return NULL;

	return monitors[0];
}
