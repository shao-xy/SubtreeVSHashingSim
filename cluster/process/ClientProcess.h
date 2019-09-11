#ifndef __CLUSTER_PROCESS_CLIENTPROCESS_H__
#define __CLUSTER_PROCESS_CLIENTPROCESS_H__

#include "cluster/Process.h"

#include "cluster/network/NetworkEntity.h"

class ClientProcess : public Process {
private:
	NetworkEntity mon;
	NetworkEntity root_mds;
	bool connected;
public:
	ClientProcess(Host * h) : Process(h), connected(false) {}
	ClientProcess(Host * h, Host * mon_host, Port mon_p) : Process(h), mon(mon_host, mon_p), connected(false) {}
	~ClientProcess() {}

	virtual bool handle_message(Message * m) override;
private:
	bool handle_findmdsack(Message * m);
public:
	bool connect_cluster();
};

#endif /* cluster/process/ClientProcess.h */
