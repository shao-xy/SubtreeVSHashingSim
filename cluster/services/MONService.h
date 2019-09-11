#ifndef __CLUSTER_SERVICES_MONSERVICE_H__
#define __CLUSTER_SERVICES_MONSERVICE_H__

#include "cluster/Service.h"

class MONService : public Service {
public:
	MONService(Host * host = NULL) : Service(host) {}

	bool entry() override;

	bool handle_message(Message * m) override;
private:
	bool handle_findmds(Message * m);
};

#endif /* cluster/services/MONService.h */
