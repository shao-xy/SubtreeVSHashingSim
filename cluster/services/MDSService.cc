#include "MDSService.h"

#include "common/Debug.h"

#include "cluster/Host.h"
#include "cluster/Cluster.h"

#include "cluster/messages/MMDSReg.h"
#include "cluster/messages/MMDSRegAck.h"
#include "cluster/messages/MClientRequest.h"
#include "cluster/messages/MClientRequestReply.h"

#include "fs/CachedObject.h"

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
	
	int type = m->get_type();
	bool ret = false;

	switch (type) {
		// monitor
		case MSG_MDSREGACK:
			ret = handle_mdsregack(m);
			break;
		// client
		case MSG_CLIENTREQ:
			ret = handle_clientrequest(m);
		default:
			break;
	}

	delete m;
	return ret;
}

bool MDSService::handle_mdsregack(Message * m)
{
	MMDSRegAck * msg = static_cast<MMDSRegAck *>(m);
	whoami = msg->rank;
	dout << __func__ << " Set my rank to " << whoami << dendl;
	return true;
}

bool MDSService::handle_clientrequest(Message * m)
{
	MClientRequest * msg = static_cast<MClientRequest *>(m);
	//return send_message(&msg->src, new MClientRequestReply(msg->path, new CInode));
	return send_message(&msg->src, new MClientRequestReply(msg->path, NULL));
}
