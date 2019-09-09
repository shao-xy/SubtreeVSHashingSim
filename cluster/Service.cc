#include "services/MONService.h"
#include "services/MDSService.h"

Service * Service::create(Host * h, string type)
{
	if (type == "mon")
		return new MONService(h);
	else if (type == "mds")
		return new MDSService(h);
	else
		return NULL;
}
