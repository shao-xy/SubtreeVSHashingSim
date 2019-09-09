#ifndef __CLUSTER_SERVICE_H__
#define __CLUSTER_SERVICE_H__

#include "include/types.h"

class Host;

class Service {
public:
	enum Type {
		SERVICE_MON,
		SERVICE_MDS,
	};
protected:
	Host * host;
	Type type;
	Service(Host * h) : host(h) {}
public:
	static Service * create(Host * h, string type);
	virtual ~Service() {}

	virtual bool start() = 0; 
	virtual bool stop() = 0;
};

#endif /* cluster/Service.h */
