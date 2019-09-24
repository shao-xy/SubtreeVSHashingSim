#ifndef __CLUSTER_MESSAGES_MMDSMAPUPDATE_H__
#define __CLUSTER_MESSAGES_MMDSMAPUPDATE_H__

#include "cluster/network/Message.h"

class MMDSMapUpdate : public Message {
public:
	map<MDSRank, NetworkEntity *> mds_active_list;

	MMDSMapUpdate(map<MDSRank, NetworkEntity *> mds_active_list) : Message(MSG_MDSMAPUPDATE), mds_active_list(mds_active_list) {}
	~MMDSMapUpdate() override {}

	string get_type_name() { return "MMDSMapUpdate"; }
};

#endif /* cluster/messages/MMDSMapUpdate.h */
