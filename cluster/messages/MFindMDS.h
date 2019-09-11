#ifndef __CLUSTER_MESSAGES_MFINDMDS_H__
#define __CLUSTER_MESSAGES_MFINDMDS_H__

#include "cluster/network/Message.h"

class MFindMDS : public Message {
public:
	MFindMDS() : Message(MSG_FINDMDS) {}
	MFindMDS(NetworkEntity from, NetworkEntity to) :
		Message(MSG_FINDMDS, from, to) {}
	~MFindMDS() {}
};

#endif /* cluster/messages/MFindMDS.h */
