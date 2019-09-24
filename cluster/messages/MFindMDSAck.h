#ifndef __CLUSTER_MESSAGES_MFINDMDSAck_H__
#define __CLUSTER_MESSAGES_MFINDMDSAck_H__

#include "cluster/network/Message.h"
#include "cluster/services/mds/mdstypes.h"

class NetworkEntity;

class MFindMDSAck : public Message {
public:
	map<MDSRank, NetworkEntity *> mds_active_list;

	MFindMDSAck() : Message(MSG_FINDMDSACK) {}
	MFindMDSAck(map<MDSRank, NetworkEntity *> mds_active_list) : Message(MSG_FINDMDSACK), mds_active_list(mds_active_list) {}
	~MFindMDSAck() {}

	string get_type_name() { return "MFindMDSAck"; }
};

#endif /* cluster/messages/MFindMDSAck.h */
