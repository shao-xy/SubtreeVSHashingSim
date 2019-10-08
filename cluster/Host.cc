#include "Host.h"
#include "Cluster.h"

#include "common/Debug.h"
#include "time/Time.h"

#include "Control.h"

#undef dout_prefix
#define dout_prefix "Host." << name() << ' '

Host::~Host()
{
	for (Process * proc : process_list) {
		delete proc;
	}

	for (map<Port, Service *>::iterator it = network.service_map.begin(); it != network.service_map.end(); it++) {
		delete it->second;
	}
}

bool Host::attach(Process * proc)
{
	if (!proc)	return false;
	if (proc->get_host() != this)	return false;

	process_list.push_back(proc);
	return true;
}

Port Host::register_service(Service * s)
{
	Port p = network.register_service(s);
	if (cluster && s->get_type() == Service::SERVICE_MON) {
		cluster->register_monitor(new NetworkEntity(this, p));
	}
	return p;
}

bool Host::unregister_service(Port port)
{
	return network.unregister_service(port);
}

Port Host::find_or_register_rev(Process * proc)
{
	return network.find_or_register_rev(proc);
}

bool Host::unregister_procrev(Port p)
{
	return network.unregister_procrev(p);
}

bool Host::send_message(Process * proc, NetworkEntity * target, Message * m)
{
	if (!target || !target->host || target->host < 0)	return false;

	dout << __func__ << " Host " << name() << " send message type " << m->get_type_name() << " to " << (*target) << dendl;

	Port revport = find_or_register_rev(proc);

	m->src.host = this;
	m->src.port = revport;

	m->dst.host = target->host;
	m->dst.port = target->port;

	if (this == target->host)
		gsw.tick_random(NETWORK_LAT_SAME_HOST);
	else
		gsw.tick_random(NETWORK_LAT_OTHER_HOST);

	bool ret = target->host->recv_message(m);

	unregister_procrev(revport);

	return ret;
}

bool Host::recv_message(Message * m)
{
	dout << __func__ << " Host " << name() << " received message type " << m->get_type_name() << " from " << m->src << dendl;
	return network.recv_message(m);
}
