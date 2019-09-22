#ifndef __CLUSTER_MESSAGES_MMDSREG_H__
#define __CLUSTER_MESSAGES_MMDSREG_H__

#include "cluster/network/Message.h"

class MMDSReg : public Message {
public:
	Port port;

	MMDSReg(Port port = -1) : Message(MSG_MDSREG), port(port) {}
	~MMDSReg() override {}

	string get_type_name() { return "MMDSReg"; }
};

#endif /* cluster/messages/MMDSReg.h */
