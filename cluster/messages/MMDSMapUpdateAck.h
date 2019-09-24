#ifndef __CLUSTER_MESSAGES_MMDSMAPUPDATEACK_H__
#define __CLUSTER_MESSAGES_MMDSMAPUPDATEACK_H__

#include "cluster/network/Message.h"

#include "cluster/services/MDSService.h"

class MMDSMapUpdateAck : public Message {
public:
	MMDSMapUpdateAck() : Message(MSG_MDSMAPUPDATEACK) {}
	~MMDSMapUpdateAck() override {}

	string get_type_name() { return "MMDSMapUpdateAck"; }
};

#endif /* cluster/messages/MMDSMapUpdateAck.h */
