#ifndef __CLUSTER_NETWORK_NETWORKENTITY_H__
#define __CLUSTER_NETWORK_NETWORKENTITY_H__

#include <ostream>
using std::ostream;

typedef int Port;

class Host;

class NetworkEntity {
public:
	Host * host;
	Port port;

	NetworkEntity() : host(NULL), port(-1) {}
	NetworkEntity(Host * host, Port port) : host(host), port(port) {}
};

ostream & operator<<(ostream & os, NetworkEntity & entity);

#endif /* cluster/network/NetworkEntity.h */
