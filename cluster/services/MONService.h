#ifndef __CLUSTER_SERVICES_MONSERVICE_H__
#define __CLUSTER_SERVICES_MONSERVICE_H__

#include "cluster/Service.h"

class MONService : public Service {
public:
	MONService(Host * host = NULL) : Service(host) {}

	bool start() override;
	bool stop() override;
};

#endif /* cluster/services/MONService.h */
