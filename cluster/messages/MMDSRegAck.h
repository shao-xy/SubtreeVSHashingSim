#ifndef __CLUSTER_MESSAGES_MMDSREGACK_H__
#define __CLUSTER_MESSAGES_MMDSREGACK_H__

#include "cluster/network/Message.h"

#include "cluster/services/MDSService.h"

class MMDSRegAck : public Message {
public:
	MDSRank rank;
	MMDSRegAck(MDSRank rank = -1) : Message(MSG_MDSREGACK), rank(rank) {}
	~MMDSRegAck() override {}

	string get_type_name() { return "MMDSRegAck"; }
};

#endif /* cluster/messages/MMDSRegAck.h */
