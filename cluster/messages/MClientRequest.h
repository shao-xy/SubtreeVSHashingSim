#ifndef __CLUSTER_MESSAGES_MCLIENTREQUEST_H__
#define __CLUSTER_MESSAGES_MCLIENTREQUEST_H__

#include <string>
using std::string;

#include "cluster/network/Message.h"

#include "fs/CachedObject.h"

enum class msg_op_fs_t {
	_NULL = 0,
	LOOKUP,
	CREATE,
	READ,
	WRITE,
	RM,
	MKDIR,
	RMDIR,
	LISTDIR,
};

class MClientRequest : public Message {
public:
	string path;
	CInode * inode;
	msg_op_fs_t op_type;
	string data;

	MClientRequest() : Message(MSG_CLIENTREQ) {}
	MClientRequest(string path) : Message(MSG_CLIENTREQ), path(path) {}
	MClientRequest(CInode * ino) : Message(MSG_CLIENTREQ), inode(ino) {}
	MClientRequest(string path, CInode * ino) : Message(MSG_CLIENTREQ), path(path), inode(ino) {}
	~MClientRequest() {}

	string get_path() { return path; }
	CInode * get_inode() { return inode; }

	msg_op_fs_t get_op() { return op_type; }
	void set_op(msg_op_fs_t op) { op_type = op; }

	string & get_data() { return data; }
	void set_data(string newdata) { data = newdata; }

	string get_type_name() { return "MClientRequest"; }
};

#endif /* cluster/messages/MClientRequest.h */
