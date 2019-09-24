#ifndef __CLUSTER_SERVICES_MONSERVICE_H__
#define __CLUSTER_SERVICES_MONSERVICE_H__

#include "include/types.h"

#include "cluster/Service.h"

#include "mds/mdstypes.h"

class MONService : public Service {
private:
	map<MDSRank, NetworkEntity *> mds_active_list;
	vector<NetworkEntity *> mds_standby_list;
	MDSRank add_mds(NetworkEntity * new_mds);
public:
	MONService(Host * host = NULL) : Service(host) {}

	bool entry() override;

	map<MDSRank, NetworkEntity *>::iterator mds_begin() { return mds_active_list.begin(); }
	map<MDSRank, NetworkEntity *>::iterator mds_end() { return mds_active_list.end(); }
	size_t mds_active_size() { return mds_active_list.size(); }

	bool handle_message(Message * m) override;
private:
	bool handle_findmds(Message * m);
	bool handle_mdsregister(Message * m);
};

#endif /* cluster/services/MONService.h */
