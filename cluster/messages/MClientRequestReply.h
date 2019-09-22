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
	vector<CInode *> inode_list;

	MClientRequestReply() : Message(MSG_CLIENTREQREPLY) {}
	MClientRequestReply(CInode * inode) : Message(MSG_CLIENTREQREPLY), inode(inode) {}
	MClientRequestReply(string path, CInode * inode) : Message(MSG_CLIENTREQREPLY), path(path), inode(inode) {}
	~MClientRequestReply() {
		for (CInode * ino : inode_list) {
			delete ino;
		}
	}

	string get_type_name() { return "MClientRequestReply"; }
};

#endif /* cluster/messages/MClientRequestReply.h */
