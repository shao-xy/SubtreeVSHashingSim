#ifndef __CLUSTER_CLUSTER_H__
#define __CLUSTER_CLUSTER_H__

#include "include/types.h"

#include "Host.h"

class Cluster {
public:
	Cluster() {}
	~Cluster();

	Host * add_host(string name);
	vector<Host *>::iterator begin() { return hosts.begin(); }
	vector<Host *>::iterator end() { return hosts.end(); }
	
	bool register_monitor(NetworkEntity * newEntity);
	bool unregister_monitor(NetworkEntity * monEntity);

	NetworkEntity * get_monitor();
private:
	vector<NetworkEntity *> monitors;
	vector<Host *> hosts;
};

#endif /* cluster/Cluster.h */
