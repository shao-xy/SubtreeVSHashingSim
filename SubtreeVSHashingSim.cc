#include <string>

#include "common/Debug.h"

#undef dout_prefix
#define dout_prefix "Testmain "

#include "cluster/Cluster.h"
#include "cluster/process/ClientProcess.h"

#include "cluster/services/MONService.h"

#include "global/global_disp.h"

class TestClientProcess : public ClientProcess {
public:
	TestClientProcess(Host * h, Host * mon_host, Port mon_p) : ClientProcess(h, mon_host, mon_p) {}
	bool handle_message(Message * m) override;
protected:
	bool entry() override;
};

bool TestClientProcess::handle_message(Message * m)
{
	return ClientProcess::handle_message(m);
}

bool TestClientProcess::entry()
{
	dout << "Client Process start." << dendl;

	connect_cluster();

	for (int i = 0; i < 10; i++) {
		string dirpath = "/" + std::to_string(i);
		cout << "Creating directory " << dirpath << dendl;
		mkdir(dirpath);
		for (int j = 0; j < 10000; j++) {
			string filepath = dirpath + "/" + std::to_string(j);
			cout << "Creating file " << filepath << dendl;
			mknod(filepath);
		}
	}

	return true;
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

	dout << __func__ << " Creating dispatchers ..." << dendl;
	global_create_dispatchers(static_cast<MONService *>(mon_service));

	Host * client_host = c.add_host("client");
	Process * client_proc = new TestClientProcess(client_host, mon_host, mon_port);
	client_host->attach(client_proc);
	client_proc->start();

	return 0;
}

/*
 *
bool TestClientProcess_test1_entry()
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
	lsdir("/a/b", v);

	dout << "List directory: /a/b" << dendl;
	for (string s : v) {
		dout << s << dendl;
	}

	write("/d", "Test text.");
	string s;
	read("/d", s);

	dout << "Text in /d: " << s << dendl;
	
	dout << "Client Process end." << dendl;
	return true;
}
*/
