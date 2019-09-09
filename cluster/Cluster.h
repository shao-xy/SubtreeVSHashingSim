#ifndef __CLUSTER_CLUSTER_H__
#define __CLUSTER_CLUSTER_H__

#include "Host.h"

class Cluster {
public:
	Cluster() {}
	~Cluster();

	Host * add_host(string name);
	vector<Host *>::iterator begin() { return hosts.begin(); }
	vector<Host *>::iterator end() { return hosts.end(); }
private:
	vector<Host *> hosts;
};

#endif /* cluster/Cluster.h */
