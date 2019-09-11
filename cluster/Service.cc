#include "Host.h"

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

bool Service::start()
{
	Port p = get_host()->register_service(this);
	if (p < 0)	return false;

	port = p;
	return Process::start();
}

bool Service::stop()
{
	get_host()->unregister_service(port);
	return Process::stop();
}
