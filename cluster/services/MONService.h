#ifndef __CLUSTER_SERVICES_MONSERVICE_H__
#define __CLUSTER_SERVICES_MONSERVICE_H__

#include "cluster/Service.h"

class MONService : public Service {
private:
	vector<NetworkEntity *> mdses;
	void add_mds(NetworkEntity * new_mds);
public:
	MONService(Host * host = NULL) : Service(host) {}

	bool entry() override;

	NetworkEntity * get_root_mds();

	bool handle_message(Message * m) override;
private:
	bool handle_findmds(Message * m);
	bool handle_mdsregister(Message * m);
};

#endif /* cluster/services/MONService.h */
