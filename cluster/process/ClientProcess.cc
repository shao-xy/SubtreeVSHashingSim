#include "common/Debug.h"
#include "common/util.h"

#include "cluster/Host.h"

#include "cluster/messages/MFindMDS.h"
#include "cluster/messages/MFindMDSAck.h"
#include "cluster/messages/MClientRequestReply.h"

#include "fs/CachedObject.h"

#include "ClientProcess.h"

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

	//if (!msg->inode)	return false;

	/*
	if (msg->inode && msg->inode->get()) {
		dout << __func__ << " Get request reply: path = \"" << msg->path << "\", inode = " << *(msg->inode->get()) << dendl;
	}
	else
		dout << __func__ << " Get request reply: path = \"" << msg->path << "\"" << dendl;
	*/

	//cache.set(msg->path, msg->inode);

	callback_inoptr = msg->inode;
	for (CInode * ino : msg->inode_list) {
		callback_inolist.push_back(ino);
	}

	return true;
}

bool ClientProcess::connect_cluster()
{
	dout << __func__ << " Trying to connect to cluster" << dendl;

	if (connected)	return true;

	connected = send_message(&mon, new MFindMDS());

	return connected;
}

bool ClientProcess::send_request(msg_op_fs_t op, CInode * ino, string data)
{
	return send_request(op, string(""), ino, data);
}

bool ClientProcess::send_request(msg_op_fs_t op, string path, CInode * ino, string data)
{
	if (path == "" && !ino)	return false;

	MClientRequest * m;
	if (path == "")
		m = new MClientRequest(ino);
	else if (!ino)
		m = new MClientRequest(path);
	else
		m = new MClientRequest(path, ino);

	m->set_op(op);

	if (data != "")
		m->set_data(data);

	// prepare for return value
	callback_inoptr = NULL;
	callback_inolist.clear();

	// TODO: Send to which MDS?
	return send_message(&root_mds, m);
}

CInode * ClientProcess::_lookup(string path)
{
	CInode * ino = local_lookup(path);
	if (ino)	return ino;

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
	return send_request(msg_op_fs_t::LOOKUP, path);
}

bool ClientProcess::mknod(string path)
{
	string dname = ::dirname(path);
	CInode * parent = lookup(dname);
	if (!parent)	return false;

	return send_request(msg_op_fs_t::CREATE, ::basename(path), parent);
}

bool ClientProcess::rm(string path)
{
	CInode * ino = lookup(path);
	if (!ino)	return false;

	return send_request(msg_op_fs_t::RM, ino);
}

bool ClientProcess::read(string path, string & out)
{
	CInode * ino = lookup(path);
	if (!ino)	return false;

	return send_request(msg_op_fs_t::READ, ino);
}

bool ClientProcess::write(string path, string & in)
{
	CInode * ino = lookup(path);
	if (!ino)	return false;

	return send_request(msg_op_fs_t::WRITE, ino, in);
}

bool ClientProcess::mkdir(string path)
{
	CInode * ino = lookup(dirname(path));
	if (!ino)	return false;

	return send_request(msg_op_fs_t::MKDIR, basename(path), ino);
}

bool ClientProcess::rmdir(string path)
{
	CInode * ino = lookup(dirname(path));
	if (!ino)	return false;

	return send_request(msg_op_fs_t::RMDIR, basename(path), ino);
}

bool ClientProcess::_lsdir(string path)
{
	CInode * ino = lookup(dirname(path));
	if (!ino)	return false;

	return send_request(msg_op_fs_t::LISTDIR, ino);
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

