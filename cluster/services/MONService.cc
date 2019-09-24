#include <algorithm>

#include "MONService.h"

#include "common/Debug.h"

#include "cluster/Host.h"
#include "cluster/Cluster.h"

#include "cluster/messages/MFindMDSAck.h"
#include "cluster/messages/MMDSReg.h"
#include "cluster/messages/MMDSRegAck.h"
#include "cluster/messages/MMDSMapUpdate.h"
#include "cluster/messages/MMDSMapUpdateAck.h"

#undef dout_prefix
#define dout_prefix get_host()->name() << "::MONService "

bool MONService::entry()
{
	return get_host()->get_cluster()->register_monitor(new NetworkEntity(get_host(), get_port()));
}

MDSRank MONService::add_mds(NetworkEntity * new_mds)
{
	if (!new_mds)	return -1;

	MDSRank max_rank = mds_active_list.size();
	MDSRank rank = 0;
	//map<MDSRank, NetworkEntity *>::iterator it_m = std::find_if(mds_active_list.begin(), mds_active_list.end(), [=] (const map<MDSRank, NetworkEntity *>::value_type item) { return *(item.second) == (*new_mds); });
	//if (it_m == mds_active_list.end())
	//	mds_active_list.insert(std::make_pair(mds_active_list.size(), new_mds));
	for (; rank < max_rank; rank++) {
		if (!mds_active_list.count(rank))	break;
	}
	mds_active_list.insert(std::make_pair(rank, new_mds));

	vector<NetworkEntity *>::iterator it_sbm = std::find_if(mds_standby_list.begin(), mds_standby_list.end(), [=] (NetworkEntity * entity) { return (*entity) == (*new_mds); });
	if (it_sbm != mds_standby_list.end())
		mds_standby_list.erase(it_sbm);

	return rank;
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
		case MSG_MDSMAPUPDATEACK:
			ret = true; // it's OK now
			break;
		default: break;
	};
	
	delete m;
	return ret;
}

bool MONService::handle_findmds(Message * m)
{
	dout << __func__ << " Get find mds request from " << m->src << dendl;
	return send_message(&m->src, new MFindMDSAck(mds_active_list));
}

bool MONService::handle_mdsregister(Message * m)
{
	MMDSReg * msg = static_cast<MMDSReg *>(m);
	dout << __func__ << " Get register MDS at port " << msg->port << " from " << m->src << dendl;

	MDSRank rank = add_mds(new NetworkEntity(m->src.host, msg->port));
	if (rank < 0)	return false;

	MMDSRegAck * mack = new MMDSRegAck(rank);
	if (!send_message(&m->src, mack))	return false;

	for (auto it = mds_active_list.begin(); it != mds_active_list.end(); it++) {
		MMDSMapUpdate * m = new MMDSMapUpdate(mds_active_list);
		if (!send_message(it->second, m)) {
			// TODO: If failure happens, actually we need to rollback this operation
			// Assume failure never happens here
			return false;
		}
	}

	return true;
}
