#include "Host.h"

#include "process/ClientProcess.h"

Process * Process::create(Host * h, string type)
{
	//if (type == "client")
	//	return new ClientProcess(h);
	//else
		return NULL;
}

bool Process::start()
{
	st = RUNNING;
	return entry();
}

bool Process::stop()
{
	if (st == RUNNING) {
		st = STOPPED;
	}
	return true;
}

bool Process::send_message(NetworkEntity * target, Message * m)
{
	return get_host()->send_message(this, target, m);
}
