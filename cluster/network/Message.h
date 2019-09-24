#ifndef __CLUSTER_NETWORK_MESSAGE_H__
#define __CLUSTER_NETWORK_MESSAGE_H__

#include "NetworkEntity.h"

#define MSG_NULL 0

// Monitor <-> MDS
#define MSG_FINDMDS 100
#define MSG_FINDMDSACK 101
#define MSG_MDSREG 102
#define MSG_MDSREGACK 103
#define MSG_MDSMAPUPDATE 104
#define MSG_MDSMAPUPDATEACK 105

// Client <-> MDS
#define MSG_CLIENTREQ 200
#define MSG_CLIENTREQREPLY 201

// MDS <-> MDS
#define MSG_CLIENTREQFORWARD 301

class Message {
private:
	int type;
public:
	NetworkEntity src;
	NetworkEntity dst;

	Message(int type = MSG_NULL) : type(type), src(NULL, -1), dst(NULL, -1) {}
	Message(int type, Host * h_from, Port p_from, Host * h_to, Port p_to) :
		type(type), src(h_from, p_from), dst(h_to, p_to) {}
	Message(int type, NetworkEntity from, NetworkEntity to) : type(type), src(from), dst(to) {}
	virtual ~Message() {}

	int get_type() { return type; }
	virtual string get_type_name() = 0;
};

#endif /* cluster/network/Message.h */
