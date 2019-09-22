#ifndef __CLUSTER_SERVICES_MDSSERVICE_H__
#define __CLUSTER_SERVICES_MDSSERVICE_H__

#include <include/types.h>

#include "cluster/Service.h"

typedef int MDSRank;

class CInode;
class MClientRequest;

class MDSService : public Service {
private:
	MDSRank whoami;
	unordered_map<string, CInode *> cache;
public:
	MDSService(Host * host = NULL) : Service(host) {}

	MDSRank get_nodeid() { return whoami; }

	bool entry() override;

	bool handle_message(Message * m) override;
private:
	bool handle_mdsregack(Message * m);
	bool handle_clientrequest(Message * m);

	bool handle_op_lookup(MClientRequest * m);
	bool handle_op_create(MClientRequest * m);
	bool handle_op_read(MClientRequest * m);
	bool handle_op_write(MClientRequest * m);
	bool handle_op_rm(MClientRequest * m);
	bool handle_op_mkdir(MClientRequest * m);
	bool handle_op_rmdir(MClientRequest * m);
	bool handle_op_listdir(MClientRequest * m);
};

#endif /* cluster/services/MDSService.h */
