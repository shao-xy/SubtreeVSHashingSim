#include "MONService.h"

#include "common/Debug.h"

#include "cluster/Host.h"
#include "cluster/Cluster.h"

#include "cluster/messages/MFindMDSAck.h"
#include "cluster/messages/MMDSReg.h"
#include "cluster/messages/MMDSRegAck.h"

#undef dout_prefix
#define dout_prefix get_host()->name() << "::MONService "

bool MONService::entry()
{
	return get_host()->get_cluster()->register_monitor(new NetworkEntity(get_host(), get_port()));
}

void MONService::add_mds(NetworkEntity * new_mds)
{
	for (NetworkEntity * mds : mdses) {
		if ((*mds) == (*new_mds))	return;
	}

	mdses.push_back(new_mds);
}

NetworkEntity * MONService::get_root_mds()
{
	if (mdses.size() == 0)	return NULL;

	return mdses[0];
}

bool MONService::handle_message(Message * m)
{
	if (!m)	return false;

	int type = m->get_type();

	bool ret = false;
	
	switch (type) {
		case MSG_FINDMDS:
			ret = handle_findmds(m);
			break;
		case MSG_MDSREG:
			ret = handle_mdsregister(m);
			break;
		default: break;
	};
	
	delete m;
	return ret;
}

bool MONService::handle_findmds(Message * m)
{
	dout << __func__ << " Get find mds request from " << m->src << dendl;
	MFindMDSAck * mack = new MFindMDSAck();
	mack->root_mds = get_root_mds();
	return send_message(&m->src, mack);
}

bool MONService::handle_mdsregister(Message * m)
{
	MMDSReg * msg = static_cast<MMDSReg *>(m);
	dout << __func__ << " Get register MDS at port " << msg->port << " from " << m->src << dendl;

	add_mds(new NetworkEntity(m->src.host, msg->port));

	return true;
}
