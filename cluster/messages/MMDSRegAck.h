#ifndef __CLUSTER_MESSAGES_MMDSREGACK_H__
#define __CLUSTER_MESSAGES_MMDSREGACK_H__

#include "cluster/network/Message.h"

class MMDSRegAck : public Message {
public:
	MMDSRegAck() : Message(MSG_MDSREGACK) {}
	~MMDSRegAck() override {}
};

#endif /* cluster/messages/MMDSRegAck.h */
