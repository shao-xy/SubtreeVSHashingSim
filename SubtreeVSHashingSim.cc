#include <string>

#include "common/Debug.h"

#undef dout_prefix
#define dout_prefix "Testmain "

#include "cluster/Cluster.h"
#include "cluster/process/ClientProcess.h"

class TestClientProcess : public ClientProcess {
public:
	TestClientProcess(Host * h, Host * mon_host, Port mon_p) : ClientProcess(h, mon_host, mon_p) {}
	bool handle_message(Message * m) override;
protected:
	bool entry() override;
};

bool TestClientProcess::entry()
{
	dout << "Client Process start." << dendl;

	connect_cluster();

	bool ret;
	ret = mkdir("/a");
	dout << "Create directory: /a -> " << (ret ? "success" : "failed") << dendl;
	ret = mkdir("/a/b");
	dout << "Create directory: /a/b -> " << (ret ? "success" : "failed") << dendl;
	ret = mknod("/a/b/c");
	dout << "Create file: /a/b/c -> " << (ret ? "success" : "failed") << dendl;
	ret = mknod("/d");
	dout << "Create file /d -> " << (ret ? "success" : "failed") << dendl;

	vector<string> v;
	lsdir("/", v);

	dout << "List directory: /" << dendl;
	for (string s : v) {
		dout << s << dendl;
	}
	
	dout << "Client Process end." << dendl;
	return true;
}

bool TestClientProcess::handle_message(Message * m)
{
	return ClientProcess::handle_message(m);
}

int main(int argc, char ** argv)
{
	Cluster c;
	
	// add Monitor
	Host * mon_host = c.add_host("monitor");
	Service * mon_service = Service::create(mon_host, "mon");
	mon_service->start();
	Port mon_port = mon_service->get_port();

	dout << __func__ << " Registered monitor on " << mon_host->name() << " port " << mon_port << dendl;

	for (int i = 0; i < 3; i++) {
		Host * mds_host = c.add_host("mdshost." + std::to_string(i));
		for (int j = 0; j < 3; j++) {
			Service * s = Service::create(mds_host, "mds");
			s->start();
		}
	}

	Host * client_host = c.add_host("client");
	Process * client_proc = new TestClientProcess(client_host, mon_host, mon_port);
	client_host->attach(client_proc);
	client_proc->start();

	return 0;
}
