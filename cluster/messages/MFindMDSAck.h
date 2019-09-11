#ifndef __CLUSTER_MESSAGES_MFINDMDSAck_H__
#define __CLUSTER_MESSAGES_MFINDMDSAck_H__

#include "cluster/network/Message.h"

class MFindMDSAck : public Message {
public:
	NetworkEntity root_mds;

	MFindMDSAck() : Message(MSG_FINDMDSACK) {}
	MFindMDSAck(NetworkEntity from, NetworkEntity to) :
		Message(MSG_FINDMDSACK, from, to) {}
	~MFindMDSAck() {}
};

#endif /* cluster/messages/MFindMDSAck.h */
