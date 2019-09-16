#ifndef __CLUSTER_MESSAGES_MCLIENTREQUESTREPLY_H__
#define __CLUSTER_MESSAGES_MCLIENTREQUESTREPLY_H__

#include <string>
using std::string;

#include "cluster/network/Message.h"

class CInode;

class MClientRequestReply : public Message {
public:
	string path;
	CInode * inode;

	MClientRequestReply() : Message(MSG_CLIENTREQREPLY) {}
	MClientRequestReply(string & path, CInode * inode) : Message(MSG_CLIENTREQREPLY), path(path), inode(inode) {}
	~MClientRequestReply() {}
};

#endif /* cluster/messages/MClientRequestReply.h */
