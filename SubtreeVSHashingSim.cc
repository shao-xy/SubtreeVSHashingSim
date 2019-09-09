#include <string>

#include "cluster/Cluster.h"

int main(int argc, char ** argv)
{
	Cluster c;
	Host * client_host = c.add_host("client");
	for (int i = 0; i < 3; i++) {
		Host * mds_host = c.add_host("mdshost." + std::to_string(i));
		for (int j = 0; j < 3; j++) {
			Service * s = mds_host->create_service("mds");
			s->start();
		}
	}

	return 0;
}
