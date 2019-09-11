#ifndef __CLUSTER_PROCESS_H__
#define __CLUSTER_PROCESS_H__

#include "include/types.h"

class Host;
class Message;
class NetworkEntity;

class Process {
public:
	enum Status {
		LOADED, RUNNING, FAILED, STOPPED, DEAD
	};

	static Process * create(Host * h, string type);

	Process(Host * h = NULL) : host(h), st(LOADED) {}
	virtual ~Process() {}
	virtual bool start();
	virtual bool stop();

	virtual bool send_message(NetworkEntity * target, Message * m);
	virtual bool handle_message(Message * m) = 0;

	Host * get_host() { return host; }
	Status get_status() { return st; }
protected:
	Host * host;
	Status st;

	virtual bool entry() = 0; 
	virtual bool cleanup() { return true; }
};

#endif /* cluster/Process.h */
