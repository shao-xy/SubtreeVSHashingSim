#ifndef __CLUSTER_SERVICE_H__
#define __CLUSTER_SERVICE_H__

#include "Process.h"

#include "network/NetworkEntity.h"

class Message;

class Service : public Process {
public:
	enum Type {
		SERVICE_NULL,
		SERVICE_MON,
		SERVICE_MDS,
	};
protected:
	Type type;
	Port port;
	Service(Host * h) : Process(h), type(SERVICE_NULL), port(-1) {}
public:
	static Service * create(Host * h, string type);
	virtual ~Service() {}

	Port get_port() { return port; }

	bool start() override;
	bool stop() override;
};

#endif /* cluster/Service.h */
