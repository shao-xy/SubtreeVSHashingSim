#include <algorithm>

#include "Network.h"

#include "common/Debug.h"

#include "cluster/Service.h"

Port Network::register_service(Service * s)
{
	if (!s)	return -1;

	// find a port for this service
	Port max_port = service_map.size();
	int i = max_port;
	for (; i > -1; i--) {
		if (!service_map.count(i)) break;
	}

	if (i != -1) {
		service_map.insert(std::make_pair(i, s));
	} else {
		dout << __func__ << "Unexpected no port for service. " << dendl;
	}

	return i;
}

bool Network::unregister_service(Port port)
{
	map<Port, Service *>::iterator it_s = service_map.find(port);
	if (it_s == service_map.end())	return false;

	service_map.erase(it_s);
	return true;
}

Port Network::find_procrev(Process * p)
{
	map<Port, Process *>::iterator it_s = std::find_if(proc_revmap.begin(), proc_revmap.end(), [p] (const map<Port, Process *>::value_type item) -> bool { return item.second == p; });
	return it_s != proc_revmap.end() ? it_s->first : -1;
}

Port Network::register_procrev(Process * p)
{
	if (!p)	return -1;

	// find a port for this service
	Port max_port =  NETWORK_PORT_PROC_REVMAP_BASE + proc_revmap.size();
	int i = max_port;
	for (; i >  NETWORK_PORT_PROC_REVMAP_BASE - 1; i--) {
		if (!proc_revmap.count(i)) break;
	}

	if (i >= NETWORK_PORT_PROC_REVMAP_BASE ) {
		proc_revmap.insert(std::make_pair(i, p));
	} else {
		dout << __func__ << "Unexpected no port for process. " << dendl;
		i = -1;
	}

	return i;
}

Port Network::find_or_register_rev(Process * p)
{
	Port port = find_procrev(p);
	return port < 0 ? register_procrev(p) : port;
}

bool Network::unregister_procrev(Port port)
{
	map<Port, Process *>::iterator it_p = proc_revmap.find(port);
	if (it_p == proc_revmap.end())	return false;

	proc_revmap.erase(it_p);
	return true;
}

bool Network::recv_message(Message * m)
{
	if (!m)	return false;
	if (m->dst.host != get_host()) { return false; } // not mine

	return dispatch(m);
}

bool Network::dispatch(Message * m)
{
	Port p = m->dst.port;
	if (service_map.count(p))
		return service_map[p]->handle_message(m);
	
	if (proc_revmap.count(p))
		return proc_revmap[p]->handle_message(m);

	return false;
}
