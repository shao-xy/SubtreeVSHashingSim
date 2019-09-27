#include <algorithm>

#include "MDSService.h"

#include "common/Debug.h"

#include "cluster/Host.h"
#include "cluster/Cluster.h"

#include "cluster/messages/MMDSReg.h"
#include "cluster/messages/MMDSRegAck.h"
#include "cluster/messages/MMDSMapUpdate.h"
#include "cluster/messages/MMDSMapUpdateAck.h"
#include "cluster/messages/MClientRequest.h"
#include "cluster/messages/MClientRequestReply.h"
#include "cluster/messages/MClientRequestForward.h"
#include "cluster/messages/MMDSSlaveRequest.h"
#include "cluster/messages/MMDSSlaveRequestReply.h"

#include "fs/CachedObject.h"
#include "fs/FileSystem.h"

#include "global/global_disp.h"

#include "time/Time.h"

#undef dout_prefix
#define dout_prefix get_host()->name() << "::MDSService(rank = " << whoami << ") "

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
		case MSG_MDSMAPUPDATE:
			ret = handle_mdsmapupdate(m);
			break;
		// client
		case MSG_CLIENTREQ:
			ret = handle_clientrequest(m);
			break;
		// mds
		case MSG_CLIENTREQFORWARD:
			ret = handle_clientrequestforward(m);
			break;
		case MSG_MDSSLAVEREQ:
			ret = handle_slaverequest(m);
			break;
		case MSG_MDSSLAVEREQREPLY:
			ret = handle_slaverequestreply(m);
			break;
		// unknown
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

bool MDSService::handle_mdsmapupdate(Message * m)
{
	MMDSMapUpdate * msg = static_cast<MMDSMapUpdate *>(m);
	mds_map = msg->mds_active_list;
	dout << __func__ << " Update MDS map success. Total active: " << mds_map.size() << dendl;
	return send_message(&m->src, new MMDSMapUpdateAck);
}

bool MDSService::handle_clientrequest(Message * m)
{
	MClientRequest * msg = static_cast<MClientRequest *>(m);
	string fullpath = msg->get_fullpath();
	// Check if my work
	MDSRank targetrank = global_get_dispatcher_from_conf()->dispatch(fullpath);
	if (targetrank == -1) {
		dout << __func__ << " Dispatcher cannot determine which MDS should handle this request. I will do this" << dendl;
	}
	else if (targetrank != whoami) {
		dout << __func__ << " Message should be handled by rank " << targetrank << ", forwarding ... " << dendl;
		return send_message(mds_map[targetrank], new MClientRequestForward(m));
	}

	msg_op_fs_t op = msg->get_op();
	string path = msg->get_path();

	bool ret = false;

	//dout << __func__ << " Get client request op: " << op << " (path = " << path << ")" << dendl;
	dout << __func__ << " Get client request op: " << static_cast<typename std::underlying_type<msg_op_fs_t>::type>(op) << " (path = \"" << path << "\")" << dendl;

	switch (op) {
		case msg_op_fs_t::LOOKUP:
			ret = handle_op_lookup(msg);
			break;
		case msg_op_fs_t::CREATE:
			ret = handle_op_create(msg);
			break;
		case msg_op_fs_t::READ:
			ret = handle_op_read(msg);
			break;
		case msg_op_fs_t::WRITE:
			ret = handle_op_write(msg);
			break;
		case msg_op_fs_t::RM:
			ret = handle_op_rm(msg);
			break;
		case msg_op_fs_t::MKDIR:
			ret = handle_op_mkdir(msg);
			break;
		case msg_op_fs_t::RMDIR:
			ret = handle_op_rmdir(msg);
			break;
		case msg_op_fs_t::LISTDIR:
			ret = handle_op_listdir(msg);
			break;
		default:
			break;
	}

	return ret;
}

bool MDSService::handle_clientrequestforward(Message * m)
{
	MClientRequestForward * wrapper_msg = static_cast<MClientRequestForward *>(m);
	return handle_clientrequest(wrapper_msg->inner_m);
}

bool MDSService::handle_slaverequest(Message * m)
{
	MMDSSlaveRequest * msg = static_cast<MMDSSlaveRequest *>(m);
	string fullpath = msg->get_fullpath();
	// Check if my work : send to myself, stop recursive
	MDSRank targetrank = global_get_dispatcher_from_conf()->dispatch(fullpath);
	if (targetrank == -1 || targetrank == whoami)	return true;

	msg_op_fs_t op = msg->get_op();
	string path = msg->get_path();

	dout << __func__ << " Get mds slave request op: " << static_cast<typename std::underlying_type<msg_op_fs_t>::type>(op) << " (path = \"" << path << "\")" << dendl;

	bool ret = false;
	switch (op) {
		case msg_op_fs_t::RMDIR:
			ret = handle_slave_op_rmdir(msg);
			break;
		case msg_op_fs_t::LISTDIR:
			ret = handle_slave_op_listdir(msg);
			break;
		default:
			break;
	}

	return ret;
}

bool MDSService::handle_slaverequestreply(Message * m)
{
	MMDSSlaveRequestReply * msg = static_cast<MMDSSlaveRequestReply *>(m);
	dout << __func__ << " Get slave request reply on path " << msg->get_fullpath() << dendl;

	for (CInode * ino : msg->inode_list) {
		callback_inolist.push_back(new CInode(ino));
	}

	return true;
}

bool MDSService::handle_op_create(MClientRequest * m)
{
	Inode * ino = gfs.mknod(m->get_inode()->get(), m->get_path());
	if (!ino)	return false;
	CInode * inode = new CInode(ino);
	return send_message(&m->src, new MClientRequestReply(whoami, m->get_fullpath(), inode));
}

bool MDSService::handle_op_lookup(MClientRequest * m)
{
	string path = m->get_path();

	// TODO: We need to look up through the whole path
	Inode * inode = gfs.lookup(path);
	if (!inode) {
		dout << __func__ << " Look up " << path << " failed in FS" << dendl;
		return false;
	}

	return send_message(&m->src, new MClientRequestReply(whoami, m->get_fullpath(), new CInode(inode)));
}

bool MDSService::handle_op_read(MClientRequest * m)
{
	MClientRequestReply * rmsg = new MClientRequestReply(whoami, m->get_fullpath());
	if (!gfs.read(m->get_inode()->get(), rmsg->data))	return false;
	return send_message(&m->src, rmsg);
}

bool MDSService::handle_op_write(MClientRequest * m)
{
	if (!gfs.write(m->get_inode()->get(), m->get_data()))	return false;
	return send_message(&m->src, new MClientRequestReply(whoami, m->get_fullpath()));
}

bool MDSService::handle_op_rm(MClientRequest * m)
{
	if (!gfs.rm(m->get_inode()->get()))	return false;
	return send_message(&m->src, new MClientRequestReply(whoami, m->get_fullpath()));
}

bool MDSService::handle_op_mkdir(MClientRequest * m)
{
	Inode * ino = gfs.mkdir(m->get_inode()->get(), m->get_path());
	if (!ino)	return false;
	return send_message(&m->src, new MClientRequestReply(whoami, m->get_fullpath(), new CInode(ino)));
}

bool MDSService::handle_op_rmdir(MClientRequest * m)
{
	vector<Inode *> v;
	if (!gfs.listdir(m->get_inode()->get(), v))	return false;

	size_t count = 0;
	vector<MDSRank> other_targeted_mds;
	for (Inode * ino : v) {
		string fullpath = m->get_fullpath() + "/" + ino->get_name();
		MDSRank targetrank = global_get_dispatcher_from_conf()->dispatch(fullpath);
		if (targetrank != whoami && targetrank != -1) {
			if (std::find(other_targeted_mds.begin(), other_targeted_mds.end(), targetrank) == other_targeted_mds.end())
				other_targeted_mds.push_back(targetrank);
		}
		else {
			count++;
		}
	}

	for (MDSRank rank : other_targeted_mds) {
		MMDSSlaveRequest * paral_m = new MMDSSlaveRequest(m->get_inode());
		paral_m->set_fullpath(m->get_fullpath());
		paral_m->set_op(msg_op_fs_t::RMDIR);
		if (!send_message(mds_map[rank], paral_m))
			return false;
	}

	gsw.tick_random("memory", count);

	if (!gfs.rmdir(m->get_inode()->get()))	return false;
	return send_message(&m->src, new MClientRequestReply(whoami, m->get_fullpath()));
}

bool MDSService::handle_op_listdir(MClientRequest * m)
{
	vector<Inode *> v;
	if (!gfs.listdir(m->get_inode()->get(), v))	return false;

	MClientRequestReply * rmsg = new MClientRequestReply(whoami, m->get_fullpath());
	size_t count = 0;
	vector<MDSRank> other_targeted_mds;
	for (Inode * ino : v) {
		string fullpath = m->get_fullpath() + "/" + ino->get_name();
		MDSRank targetrank = global_get_dispatcher_from_conf()->dispatch(fullpath);
		if (targetrank == whoami || targetrank == -1) {
			rmsg->inode_list.push_back(new CInode(ino));
			count++;
		}
		else if (std::find(other_targeted_mds.begin(), other_targeted_mds.end(), targetrank) == other_targeted_mds.end())
			other_targeted_mds.push_back(targetrank);
	}

	gsw.tick_random("memory", count);

	for (MDSRank rank : other_targeted_mds) {
		MMDSSlaveRequest * paral_m = new MMDSSlaveRequest(m->get_inode());
		paral_m->set_fullpath(m->get_fullpath());
		paral_m->set_op(msg_op_fs_t::LISTDIR);
		callback_inolist.clear();
		if (!send_message(mds_map[rank], paral_m))
			return false;
		rmsg->inode_list.insert(rmsg->inode_list.end(), callback_inolist.begin(), callback_inolist.end());
	}

	return send_message(&m->src, rmsg);
}

bool MDSService::handle_slave_op_rmdir(MMDSSlaveRequest * m)
{
	vector<Inode *> v;
	if (!gfs.listdir(m->get_inode()->get(), v))	return false;

	MMDSSlaveRequestReply * rmsg = new MMDSSlaveRequestReply(whoami, m->get_fullpath());
	size_t count = 0;
	for (Inode * ino : v) {
		string fullpath = m->get_fullpath() + "/" + ino->get_name();
		MDSRank targetrank = global_get_dispatcher_from_conf()->dispatch(fullpath);
		if (targetrank == whoami || targetrank == -1)
			count++;
	}

	gsw.tick_random("memory", count);

	return send_message(&m->src, rmsg);
}

bool MDSService::handle_slave_op_listdir(MMDSSlaveRequest * m)
{
	vector<Inode *> v;
	if (!gfs.listdir(m->get_inode()->get(), v))	return false;

	MMDSSlaveRequestReply * rmsg = new MMDSSlaveRequestReply(whoami, m->get_fullpath());
	size_t count = 0;
	vector<MDSRank> other_targeted_mds;
	for (Inode * ino : v) {
		string fullpath = m->get_fullpath() + "/" + ino->get_name();
		MDSRank targetrank = global_get_dispatcher_from_conf()->dispatch(fullpath);
		if (targetrank == whoami || targetrank == -1) {
			rmsg->inode_list.push_back(new CInode(ino));
			count++;
		}
	}

	gsw.tick_random("memory", count);

	return send_message(&m->src, rmsg);
}
