#ifndef __CLUSTER_MESSAGES_MCLIENTREQUESTFORWARD_H__
#define __CLUSTER_MESSAGES_MCLIENTREQUESTFORWARD_H__

class MClientRequest;

class MClientRequestForward : public Message {
public:
	Message * inner_m;

	MClientRequestForward(Message * m = NULL) : Message(MSG_CLIENTREQFORWARD), inner_m(m) {}

	string get_type_name() { return "MClientRequestForward"; }
};

#endif /* cluster/messages/MClientRequestForward.h */
