#ifndef __CLUSTER_SERVICES_MDSSERVICE_H__
#define __CLUSTER_SERVICES_MDSSERVICE_H__

#include "cluster/Service.h"

class MDSService : public Service {
public:
	MDSService(Host * host = NULL) : Service(host) {}

	bool start() override;
	bool stop() override;
};

#endif /* cluster/services/MDSService.h */
