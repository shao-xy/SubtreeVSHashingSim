#include "common/Debug.h"
#include "common/util.h"

#include "cluster/Host.h"

#include "cluster/messages/MFindMDS.h"
#include "cluster/messages/MFindMDSAck.h"
#include "cluster/messages/MClientRequestReply.h"

#include "fs/CachedObject.h"

#include "global/global_conf.h"
#include "global/global_disp.h"

#include "ClientProcess.h"

#undef dout_prefix
#define dout_prefix get_host()->name() << ' '

bool ClientProcess::set_root_mds()
{
	NetworkEntity * entity = 0;
	if (mdsmap.count(0) != 0)
		entity = mdsmap[0];
	else {
		// no root mds, find one
		entity = mdsmap.size() == 0 ? NULL : mdsmap.begin()->second;
	}

	if (!entity)	return false;

	root_mds.host = entity->host;
	root_mds.port = entity->port;

	return true;
}

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

	mdsmap = msg->mds_active_list;

	if (!set_root_mds())	return false;

	//dout << __func__ << " Get known: root MDS at " << root_mds.host->name() << " port " << root_mds.port << dendl;
	dout << __func__ << " Get known: root MDS at " << root_mds << dendl;
	return true;
}

bool ClientProcess::handle_clientrequestreply(Message * m)
{
	MClientRequestReply * msg = static_cast<MClientRequestReply *>(m);

	string fullpath = msg->fullpath;
	dout << __func__ << " Get request reply of full path " << fullpath << dendl;

	MDSRank fromwho = msg->fromwho;
	routetable[fullpath] = fromwho;

	if (msg->inode)
		cache.set(fullpath, msg->inode);
	else
		cache.get(fullpath);

	callback_inoptr = msg->inode;
	for (CInode * ino : msg->inode_list) {
		callback_inolist.push_back(new CInode(ino));
	}
	callback_data = msg->data;

	return true;
}

bool ClientProcess::connect_cluster()
{
	dout << __func__ << " Trying to connect to cluster" << dendl;

	if (connected)	return true;

	connected = send_message(&mon, new MFindMDS());

	return connected;
}

MDSRank ClientProcess::findroute(string & fullpath)
{
	MDSRank ret = -1;

	if (fullpath == "")	return ret;

	if (fullpath[0] != '/')
		fullpath = "/" + fullpath;

	while (fullpath.back() == '/' && fullpath.length() > 1)	fullpath = fullpath.substr(0, fullpath.length() - 1);

	dout << __func__ << " Checking local route for path: " << fullpath << dendl;

	// return hashed target if hashing method is taken
	string dist_strategy = g_conf.get("mds_md_dist_strategy");
	if (dist_strategy == "hashing")
		return hashingDisp->dispatch(fullpath);
	else if (dist_strategy == "hybrid")
		return hybridDisp->dispatch(fullpath);

	// Fullpath
	if (routetable.count(fullpath) > 0) {
		ret = routetable[fullpath];
		dout << __func__ << " Request with full path: " << fullpath << " should be posted to rank " << ret << dendl;
		return ret;
	}

	// Longest prefix matching
	vector<unsigned int> slashes = ::findAll(fullpath, "/");
	for (auto it = slashes.rbegin(); it != slashes.rend() - 1; it++) {
		string subpath = fullpath.substr(0, *it);
		if (routetable.count(subpath) > 0) {
			ret = routetable[subpath];
			dout << __func__ << " Request with full path: " << fullpath << " should be posted to rank " << ret << dendl;
			return ret;
		}
	}
	if (routetable.count("/") > 0) {
		ret = routetable["/"];
	}
	dout << __func__ << " Request with full path: " << fullpath << " should be posted to rank " << ret << dendl;
	return ret;
}

bool ClientProcess::send_request(string fullpath, msg_op_fs_t op, CInode * ino, string data)
{
	return send_request(fullpath, op, string(""), ino, data);
}

bool ClientProcess::send_request(string fullpath, msg_op_fs_t op, string path, CInode * ino, string data)
{
	if (path == "" && !ino)	return false;

	MClientRequest * m;
	if (path == "")
		m = new MClientRequest(ino);
	else if (!ino)
		m = new MClientRequest(path);
	else
		m = new MClientRequest(path, ino);

	m->set_fullpath(fullpath);
	m->set_op(op);

	if (data != "")
		m->set_data(data);

	// prepare for return value
	callback_inoptr = NULL;
	callback_inolist.clear();

	MDSRank target = findroute(fullpath);
	return target != -1 ? send_message(mdsmap[target], m) : send_message(&root_mds, m);
}

CInode * ClientProcess::_lookup(string path)
{
	if (ENABLE_CLIENT_CACHE) {
		CInode * ino = local_lookup(path);
		if (ino)	return ino;
	}

	remote_lookup(path);
	return callback_inoptr;
}

CInode * ClientProcess::lookup(string path)
{
	if (path == "")	return NULL;

	if (path[0] != '/')
		path = "/" + path;

	dout << __func__ << " Lookup: " << path << dendl;

	vector<unsigned int> slashes = ::findAll(path, "/");

	if (!_lookup("/")){
		dout << __func__ << " Cannot lookup /" << dendl;
		return NULL;
	}

	auto it = slashes.begin();
	for (it++; it != slashes.end(); it++) {
		string dirpath = path.substr(0, *it);
		//dout << __func__ << " Path: " << dirpath << ", index:" << *it << dendl;
		if (!_lookup(dirpath)) {
			dout << __func__ << " Cannot lookup subpath: " << dirpath << dendl;
			return NULL;
		}
	}

	CInode * ino = _lookup(path);
	if (!ino) {
		dout << __func__ << " Cannot lookup: " << path << dendl;
	}
	return ino;
}

CInode * ClientProcess::local_lookup(string path)
{
	return cache.get(path);
}

bool ClientProcess::remote_lookup(string path)
{
	return send_request(path, msg_op_fs_t::LOOKUP, path);
}

bool ClientProcess::mknod(string path)
{
	string dname = ::dirname(path);
	CInode * parent = lookup(dname);
	if (!parent)	return false;

	return send_request(path, msg_op_fs_t::CREATE, ::basename(path), parent);
}

bool ClientProcess::rm(string path)
{
	CInode * ino = lookup(path);
	if (!ino)	return false;

	return send_request(path, msg_op_fs_t::RM, ino);
}

bool ClientProcess::read(string path, string & out)
{
	CInode * ino = lookup(path);
	if (!ino)	return false;

	if (!send_request(path, msg_op_fs_t::READ, ino))	return false;

	out = callback_data;
	return true;
}

bool ClientProcess::write(string path, string in)
{
	CInode * ino = lookup(path);
	if (!ino)	return false;

	return send_request(path, msg_op_fs_t::WRITE, ino, in);
}

bool ClientProcess::mkdir(string path)
{
	CInode * ino = lookup(dirname(path));
	if (!ino)	return false;

	return send_request(path, msg_op_fs_t::MKDIR, basename(path), ino);
}

bool ClientProcess::rmdir(string path)
{
	CInode * ino = lookup(path);
	if (!ino || !ino->is_dir())	return false;

	return send_request(path, msg_op_fs_t::RMDIR, ino);
}

bool ClientProcess::_lsdir(string path)
{
	CInode * ino = lookup(path);
	if (!ino)	return false;

	return send_request(path, msg_op_fs_t::LISTDIR, ino);
}

bool ClientProcess::lsdir(string path, vector<string> & ret)
{
	if (!_lsdir(path))	return false;

	for (CInode * ino : callback_inolist) {
		ret.push_back(ino->get_name());
	}
	return true;
}

bool ClientProcess::lsdir(string path, vector<CInode *> & ret)
{
	if (!_lsdir(path))	return false;

	for (CInode * ino : callback_inolist) {
		ret.push_back(ino);
	}
	return true;
}

bool ClientProcess::exist(string path)
{
	return lookup(path) != NULL;
}

bool ClientProcess::is_file(string path)
{
	CInode * ino = lookup(path);
	return ino && ino->is_file();
}

bool ClientProcess::is_dir(string path)
{
	CInode * ino = lookup(path);
	return ino && ino->is_dir();
}

