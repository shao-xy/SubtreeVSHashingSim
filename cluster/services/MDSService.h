#ifndef __CLUSTER_SERVICES_MDSSERVICE_H__
#define __CLUSTER_SERVICES_MDSSERVICE_H__

#include <include/types.h>

#include "cluster/Service.h"
#include "mds/mdstypes.h"

class CInode;
class MClientRequest;
class MMDSSlaveRequest;
class NetworkEntity;

class MDSService : public Service {
private:
	MDSRank whoami;
	map<MDSRank, NetworkEntity *> mds_map;
	unordered_map<string, CInode *> cache;

	vector<CInode *> callback_inolist;
public:
	MDSService(Host * host = NULL) : Service(host) {}

	MDSRank get_nodeid() { return whoami; }

	bool entry() override;

	bool handle_message(Message * m) override;
private:
	bool handle_mdsregack(Message * m);
	bool handle_mdsmapupdate(Message * m);
	bool handle_clientrequest(Message * m);
	bool handle_clientrequestforward(Message * m);
	bool handle_slaverequest(Message * m);
	bool handle_slaverequestreply(Message * m);

	bool handle_op_lookup(MClientRequest * m);
	bool handle_op_create(MClientRequest * m);
	bool handle_op_read(MClientRequest * m);
	bool handle_op_write(MClientRequest * m);
	bool handle_op_rm(MClientRequest * m);
	bool handle_op_mkdir(MClientRequest * m);
	bool handle_op_rmdir(MClientRequest * m);
	bool handle_op_listdir(MClientRequest * m);

	bool handle_slave_op_rmdir(MMDSSlaveRequest * m);
	bool handle_slave_op_listdir(MMDSSlaveRequest * m);
};

#endif /* cluster/services/MDSService.h */
