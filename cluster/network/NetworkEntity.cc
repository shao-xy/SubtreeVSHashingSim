#include "NetworkEntity.h"

#include "cluster/Host.h"

ostream & operator<<(ostream & os, NetworkEntity & entity)
{
	if (!entity.host)
		os << "[NetworkEntity host (null) port=" << entity.port << "]";
	else
		os << "[NetworkEntity host=\"" << entity.host->name() << "\" port=" << entity.port << "]";
	
	return os;
}
