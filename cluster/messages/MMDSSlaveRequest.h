#ifndef __CLUSTER_MESSAGES_MMDSSLAVEREQUEST_H__
#define __CLUSTER_MESSAGES_MMDSSLAVEREQUEST_H__

#include <string>
using std::string;

#include "msg_optypes.h"

#include "cluster/network/Message.h"

#include "fs/CachedObject.h"

class MMDSSlaveRequest : public Message {
public:
	string path;
	string fullpath;
	CInode * inode;
	msg_op_fs_t op_type;
	string data;

	MMDSSlaveRequest() : Message(MSG_MDSSLAVEREQ) {}
	MMDSSlaveRequest(string path) : Message(MSG_MDSSLAVEREQ), path(path) {}
	MMDSSlaveRequest(CInode * ino) : Message(MSG_MDSSLAVEREQ), inode(ino) {}
	MMDSSlaveRequest(string path, CInode * ino) : Message(MSG_MDSSLAVEREQ), path(path), inode(ino) {}
	~MMDSSlaveRequest() {}

	string get_path() { return path; }
	CInode * get_inode() { return inode; }

	string get_fullpath() { return fullpath; }
	void set_fullpath(string fullpath) { this->fullpath = fullpath; }

	msg_op_fs_t get_op() { return op_type; }
	void set_op(msg_op_fs_t op) { op_type = op; }

	string & get_data() { return data; }
	void set_data(string newdata) { data = newdata; }

	string get_type_name() { return "MMDSSlaveRequest"; }
};

#endif /* cluster/messages/MMDSSlaveRequest.h */
