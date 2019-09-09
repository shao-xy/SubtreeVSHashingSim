#include "MDSService.h"

#include "cluster/Host.h"
#include "common/Debug.h"

bool MDSService::start()
{
	dout << "MDSService::start on host " << host->name() << dendl;
	return true;
}

bool MDSService::stop()
{
	return true;
}
