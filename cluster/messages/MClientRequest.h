#ifndef __CLUSTER_MESSAGES_MCLIENTREQUEST_H__
#define __CLUSTER_MESSAGES_MCLIENTREQUEST_H__

#include <string>
using std::string;

#include "cluster/network/Message.h"

class MClientRequest : public Message {
public:
	string path;

	MClientRequest() : Message(MSG_CLIENTREQ) {}
	MClientRequest(string & path) : Message(MSG_CLIENTREQ), path(path) {}
	~MClientRequest() {}
};

#endif /* cluster/messages/MClientRequest.h */
