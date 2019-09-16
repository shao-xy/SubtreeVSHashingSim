#include "ClientProcess.h"

#include "common/Debug.h"
#include "common/util.h"

#include "cluster/Host.h"

#include "cluster/messages/MFindMDS.h"
#include "cluster/messages/MFindMDSAck.h"
#include "cluster/messages/MClientRequest.h"
#include "cluster/messages/MClientRequestReply.h"

#include "fs/CachedObject.h"

#undef dout_prefix
#define dout_prefix get_host()->name() << ' '

bool ClientProcess::handle_message(Message * m)
{
	if (!m)	return false;

	bool ret = false;
	switch (m->get_type())
	{
		case MSG_FINDMDSACK:
			ret = handle_findmdsack(m);
			break;
		case MSG_CLIENTREQREPLY:
			ret = handle_clientrequestreply(m);
		default:
			break;
	}

	delete m;
	return ret;
}

bool ClientProcess::handle_findmdsack(Message * m)
{
	MFindMDSAck * msg = static_cast<MFindMDSAck *>(m);

	root_mds.host = msg->root_mds.host;
	root_mds.port = msg->root_mds.port;

	//dout << __func__ << " Get known: root MDS at " << root_mds.host->name() << " port " << root_mds.port << dendl;
	dout << __func__ << " Get known: root MDS at " << root_mds << dendl;
	return true;
}

bool ClientProcess::handle_clientrequestreply(Message * m)
{
	MClientRequestReply * msg = static_cast<MClientRequestReply *>(m);

	if (!msg->inode)	return false;

	cache.set(msg->path, msg->inode);
	return true;
}

bool ClientProcess::connect_cluster()
{
	dout << __func__ << " Trying to connect to cluster" << dendl;

	if (connected)	return true;

	connected = send_message(&mon, new MFindMDS());

	return connected;
}

bool ClientProcess::local_visit(string path)
{
	CInode * inode = cache.get(path);
	return inode != NULL;
}

bool ClientProcess::remote_visit(string path)
{
	return send_message(&root_mds, new MClientRequest(path));
}

bool ClientProcess::visit(string path)
{
	dout << __func__ << " Visiting: " << path << dendl;

	if (!local_visit(path))
		return remote_visit(path);
	return true;
}

bool ClientProcess::visit_file(string path)
{
	if (path == "")	return false;

	if (path[0] != '/')
		path = "/" + path;

	dout << __func__ << " Visiting: " << path << dendl;

	vector<unsigned int> slashes = ::findAll(path, "/");
	
	if (!visit("/")){
		dout << __func__ << " Cannot visit /" << dendl;
		return false;
	}
	auto it = slashes.begin();
	for (it++; it != slashes.end(); it++) {
		string dirpath = path.substr(0, *it);
		dout << __func__ << " Path: " << dirpath << ", index:" << *it << dendl;
		if (!visit(dirpath)) {
			dout << __func__ << " Cannot visit subpath: " << dirpath << dendl;
			return false;
		}
	}
	if (!visit(path)){
		dout << __func__ << " Cannot visit subpath: " << path << dendl;
		return false;
	}

	return true;
}
