#include "MDSService.h"

#include "cluster/Host.h"

bool MDSService::entry()
{
	return true;
}

bool MDSService::handle_message(Message * m)
{
	if (!m)	return false;
	return true;
}
