#ifndef __CLUSTER_NETWORK_NETWORK_H__
#define __CLUSTER_NETWORK_NETWORK_H__

#include "include/types.h"

#include "Message.h"

#define NETWORK_PORT_PROC_REVMAP_BASE 100

class Host;
class Process;
class Service;

class Network {
	friend class Host;
private:
	map<Port, Service *> service_map;
	map<Port, Process *> proc_revmap;
	Host * host;
public:
	Network(Host * host = NULL) : host(host) {}
	~Network() {}

	Host * get_host() { return host; }

private:
	Port find_procrev(Process * p);
	Port register_procrev(Process * p);
public:
	Port find_or_register_rev(Process * p);
	bool unregister_procrev(Port port);
	Port register_service(Service * s);
	bool unregister_service(Port port);
	bool recv_message(Message * m);
private:
	bool dispatch(Message * m);
};

#endif /* cluster/network/Network.h */
