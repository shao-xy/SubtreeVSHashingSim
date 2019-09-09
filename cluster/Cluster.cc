#include "Cluster.h"

Cluster::~Cluster() {
	for (Host * host : hosts) {
		delete host;
	}
}
	
Host * Cluster::add_host(string name)
{
	Host * new_host = new Host(name);
	hosts.push_back(new_host);
	return new_host;
}
