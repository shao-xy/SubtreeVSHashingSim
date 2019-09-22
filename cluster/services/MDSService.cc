#include "MDSService.h"

#include "common/Debug.h"

#include "cluster/Host.h"
#include "cluster/Cluster.h"

#include "cluster/messages/MMDSReg.h"
#include "cluster/messages/MMDSRegAck.h"
#include "cluster/messages/MClientRequest.h"
#include "cluster/messages/MClientRequestReply.h"

#include "fs/CachedObject.h"
#include "fs/FileSystem.h"

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
	bool ret = false;

	MClientRequest * msg = static_cast<MClientRequest *>(m);
	msg_op_fs_t op = msg->get_op();
	string path = msg->get_path();

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

bool MDSService::handle_op_create(MClientRequest * m)
{
	Inode * ino = gfs.mknod(m->get_inode()->get(), m->get_path());
	if (!ino)	return false;
	CInode * inode = new CInode(ino);
	return send_message(&m->src, new MClientRequestReply(inode));
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

	return send_message(&m->src, new MClientRequestReply(new CInode(inode)));
}

bool MDSService::handle_op_read(MClientRequest * m)
{
	MClientRequestReply * rmsg = new MClientRequestReply();
	if (!gfs.read(m->get_inode()->get(), rmsg->data))	return false;
	return send_message(&m->src, rmsg);
}

bool MDSService::handle_op_write(MClientRequest * m)
{
	if (!gfs.write(m->get_inode()->get(), m->get_data()))	return false;
	return send_message(&m->src, new MClientRequestReply);
}

bool MDSService::handle_op_rm(MClientRequest * m)
{
	if (!gfs.rm(m->get_inode()->get()))	return false;
	return send_message(&m->src, new MClientRequestReply);
}

bool MDSService::handle_op_mkdir(MClientRequest * m)
{
	Inode * ino = gfs.mkdir(m->get_inode()->get(), m->get_path());
	if (!ino)	return false;
	return send_message(&m->src, new MClientRequestReply(new CInode(ino)));
}

bool MDSService::handle_op_rmdir(MClientRequest * m)
{
	if (!gfs.rmdir(m->get_inode()->get()))	return false;
	return send_message(&m->src, new MClientRequestReply);
}

bool MDSService::handle_op_listdir(MClientRequest * m)
{
	vector<Inode *> v;
	if (!gfs.listdir(m->get_inode()->get(), v))	return false;

	MClientRequestReply * rmsg = new MClientRequestReply;
	for (Inode * ino : v)
		rmsg->inode_list.push_back(new CInode(ino));

	return send_message(&m->src, rmsg);
}

