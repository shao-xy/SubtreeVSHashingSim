#ifndef __CLUSTER_MESSAGES_MCLIENTREQUESTREPLY_H__
#define __CLUSTER_MESSAGES_MCLIENTREQUESTREPLY_H__

#include <string>
using std::string;

#include "cluster/network/Message.h"

#include "fs/CachedObject.h"

class MClientRequestReply : public Message {
public:
	string path;
	CInode * inode;
	string data;
	string fullpath;
	vector<CInode *> inode_list;

	MClientRequestReply(string fullpath = "") : Message(MSG_CLIENTREQREPLY), inode(NULL), fullpath(fullpath) {}
	MClientRequestReply(string fullpath, CInode * inode) : Message(MSG_CLIENTREQREPLY), inode(inode), fullpath(fullpath) {}
	MClientRequestReply(string fullpath, string path, CInode * inode) : Message(MSG_CLIENTREQREPLY), path(path), inode(inode), fullpath(fullpath) {}
	~MClientRequestReply() {
		for (CInode * ino : inode_list) {
			delete ino;
		}
	}

	string get_type_name() { return "MClientRequestReply"; }
};

#endif /* cluster/messages/MClientRequestReply.h */
