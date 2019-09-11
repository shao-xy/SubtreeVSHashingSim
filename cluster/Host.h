#ifndef __CLUSTER_HOST_H__
#define __CLUSTER_HOST_H__

#include "include/types.h"

#include "Service.h"
#include "network/Network.h"

class Message;

class Host {
private:
	string _name;
	Network network;
	vector<Process *> process_list;
public:
	Host() : _name("SimHost"), network(this) {}
	Host(string name) : _name(name), network(this) {}
	~Host();

	bool attach(Process * proc);

	Port register_service(Service * s);
	bool unregister_service(Port port);

	Port find_or_register_rev(Process * proc);
	bool unregister_procrev(Port p);

	bool send_message(Process * proc, NetworkEntity * target, Message * m);
	bool recv_message(Message * m);
	
	string & name() { return _name; }
};

#endif /* cluster/Host.h */
