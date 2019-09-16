#include "NetworkEntity.h"

#include "cluster/Host.h"

bool NetworkEntity::is_valid()
{
	return host && port > 0;
}

NetworkEntity & NetworkEntity::operator=(NetworkEntity * another)
{
	if (another)
		return (*this) = (*another);
	else {
		this->host = NULL;
		this->port = -1;
		return *this;
	}
}

NetworkEntity & NetworkEntity::operator=(NetworkEntity & another)
{
	host = another.host;
	port = another.port;
	return *this;
}

bool NetworkEntity::operator==(NetworkEntity & another)
{
	return host == another.host && port == another.port;
}

ostream & operator<<(ostream & os, NetworkEntity & entity)
{
	if (!entity.host)
		os << "[NetworkEntity host (null) port=" << entity.port << "]";
	else
		os << "[NetworkEntity host=\"" << entity.host->name() << "\" port=" << entity.port << "]";
	
	return os;
}
