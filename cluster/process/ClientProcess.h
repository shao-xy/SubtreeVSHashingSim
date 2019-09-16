#ifndef __CLUSTER_PROCESS_CLIENTPROCESS_H__
#define __CLUSTER_PROCESS_CLIENTPROCESS_H__

#include <string>
using std::string;

#include "cluster/Process.h"

#include "cluster/network/NetworkEntity.h"

#include "include/LRU.h"

#define CLIENT_LRU_SIZE 1000

class CInode;

class ClientProcess : public Process {
private:
	NetworkEntity mon;
	NetworkEntity root_mds;
	bool connected;

	LRUCache<string, CInode *> cache;
public:
	ClientProcess(Host * h) : Process(h), connected(false), cache(CLIENT_LRU_SIZE) {}
	ClientProcess(Host * h, Host * mon_host, Port mon_p) : Process(h), mon(mon_host, mon_p), connected(false), cache(CLIENT_LRU_SIZE) {}
	~ClientProcess() {}

	virtual bool handle_message(Message * m) override;
private:
	bool handle_findmdsack(Message * m);
	bool handle_clientrequestreply(Message * m);
public:
	bool connect_cluster();
private:
	bool local_visit(string path);
	bool remote_visit(string path);
	bool visit(string path);
public:
	bool visit_file(string path);
};

#endif /* cluster/process/ClientProcess.h */
