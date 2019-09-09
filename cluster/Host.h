#ifndef __CLUSTER_HOST_H__
#define __CLUSTER_HOST_H__

#include "include/types.h"

#include "Service.h"

class Host {
private:
	string _name;
	vector<Service *> services;
public:
	Host() : _name("SimHost") {}
	Host(string name) : _name(name) {}
	~Host();

	Service * create_service(string type);
	string & name() { return _name; }
};

#endif /* cluster/Host.h */
