#include "MDSService.h"

#include "common/Debug.h"

#include "cluster/Host.h"
#include "cluster/Cluster.h"

#include "cluster/messages/MMDSReg.h"

#undef dout_prefix
#define dout_prefix get_host()->name() << "::MDSService(" << get_port() << ") "

bool MDSService::entry()
{
	if (!host)	return false;

	dout << __func__ << " MDS daemon start." << dendl;

	// connect to monitor
	Cluster * cluster = get_host()->get_cluster();
	if (!cluster)	return false;
	
	NetworkEntity * mon_entity = cluster->get_monitor();
	dout << __func__ << " Get monitor at " << (*mon_entity) << dendl;
	send_message(mon_entity, new MMDSReg(get_port()));

	return true;
}

bool MDSService::handle_message(Message * m)
{
	if (!m)	return false;
	return true;
}
