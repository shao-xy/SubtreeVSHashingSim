#include "Host.h"

Host::~Host()
{
	for (Service * service : services) {
		delete service;
	}
}

Service * Host::create_service(string type)
{
	Service * s = Service::create(this, type);
	services.push_back(s);
	return s;
}
