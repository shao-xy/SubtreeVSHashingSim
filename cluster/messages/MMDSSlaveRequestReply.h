#ifndef __CLUSTER_MESSAGES_MMDSSLAVEREQUESTREPLY_H__
#define __CLUSTER_MESSAGES_MMDSSLAVEREQUESTREPLY_H__

#include <string>
using std::string;

#include "cluster/network/Message.h"

#include "fs/CachedObject.h"

#include "cluster/services/mds/mdstypes.h"

class MMDSSlaveRequestReply : public Message {
public:
	string path;
	CInode * inode;
	string data;
	string fullpath;
	vector<CInode *> inode_list;

	MDSRank fromwho;

	MMDSSlaveRequestReply(MDSRank src = -1, string fullpath = "") : Message(MSG_MDSSLAVEREQREPLY), inode(NULL), fullpath(fullpath), fromwho(src) {}
	MMDSSlaveRequestReply(MDSRank src, string fullpath, CInode * inode) : Message(MSG_MDSSLAVEREQREPLY), inode(inode), fullpath(fullpath), fromwho(src) {}
	MMDSSlaveRequestReply(MDSRank src, string fullpath, string path, CInode * inode) : Message(MSG_MDSSLAVEREQREPLY), path(path), inode(inode), fullpath(fullpath), fromwho(src) {}
	~MMDSSlaveRequestReply() {
		for (CInode * ino : inode_list) {
			delete ino;
		}
	}

	string get_type_name() { return "MMDSSlaveRequestReply"; }
	string get_fullpath() { return fullpath; }
};

#endif /* cluster/messages/MMDSSlaveRequestReply.h */
