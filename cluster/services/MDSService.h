#ifndef __CLUSTER_SERVICES_MDSSERVICE_H__
#define __CLUSTER_SERVICES_MDSSERVICE_H__

#include "cluster/Service.h"

typedef int MDSRank;

class MDSService : public Service {
private:
	MDSRank whoami;
	//unordered_map<string, bool> cache;
	//unordered_map<string, CInode *> cache;
public:
	MDSService(Host * host = NULL) : Service(host) {}

	MDSRank get_nodeid() { return whoami; }

	bool entry() override;

	bool handle_message(Message * m) override;
private:
	bool handle_mdsregack(Message * m);
	bool handle_clientrequest(Message * m);
};

#endif /* cluster/services/MDSService.h */
