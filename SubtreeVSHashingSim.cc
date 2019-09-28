#include <iostream>
#include <string>

#include "common/Debug.h"

#undef dout_prefix
#define dout_prefix "Testmain "

#include "cluster/Cluster.h"
#include "cluster/process/ClientProcess.h"

#include "cluster/services/MONService.h"

#include "global/global_disp.h"
#include "global/global_conf.h"

#include "time/Time.h"

#define HOST_SIZE "5"
#define MDS_SIZE_PER_HOST "1"

#define DIR_SIZE 10
#define FILE_SIZE_PER_DIR 10000
#define FILE_SIZE (DIR_SIZE * FILE_SIZE_PER_DIR)

#define METADATA_STRATEGY "hashing"

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

	// simulate: mount
	connect_cluster();

	// simulate: create files
	for (int i = 0; i < DIR_SIZE; i++) {
		string dirpath = "/" + std::to_string(i);
		cout << "Creating directory " << dirpath << dendl;
		mkdir(dirpath);
	}
	gsw.reset_zero();
	for (int i = 0; i < DIR_SIZE; i++) {
		string dirpath = "/" + std::to_string(i);
		for (int j = 0; j < FILE_SIZE_PER_DIR; j++) {
			string filepath = dirpath + "/" + std::to_string(j);
			cout << "Creating file " << filepath << '\r';
			//cout_flush;
			mknod(filepath);
		}
		cout << dendl;
	}
	cout << "\nDone." << dendl;
	cout << "Stopwatch shows: " << gsw.get() << dendl;
	cout << "Average time: " << ((double) gsw.get()) / FILE_SIZE << " us." << dendl;

	// simulate: stat (check if file)
	gsw.reset_zero();
	for (int i = 0; i < DIR_SIZE; i++) {
		string dirpath = "/" + std::to_string(i);
		for (int j = 0; j < FILE_SIZE_PER_DIR; j++) {
			string filepath = dirpath + "/" + std::to_string(j);
			cout << "Stating file " << filepath << '\r';
			is_file(filepath);
		}
		cout << dendl;
	}
	cout << "\nDone." << dendl;
	cout << "Stopwatch shows: " << gsw.get() << dendl;
	cout << "Average time: " << ((double) gsw.get()) / FILE_SIZE << " us." << dendl;

	// simulate: mkdir
	//gsw.reset_zero();
	//for (int i = DIR_SIZE; i < (DIR_SIZE + FILE_SIZE_PER_DIR); i++) {
	//	string dirpath = "/" + std::to_string(i);
	//	cout << "Creating directory " << dirpath << '\r' << dendl;
	//	mkdir(dirpath);
	//}
	//cout << "\nDone." << dendl;
	//cout << "Stopwatch shows: " << gsw.get() << dendl;
	//cout << "Average time: " << ((double) gsw.get()) / FILE_SIZE << " us." << dendl;

	// simulate: lsdir
	gsw.reset_zero();
	for (int i = 0; i < DIR_SIZE; i++) {
		string dirpath = "/" + std::to_string(i);
		cout << "Listing directory " << dirpath << dendl;
		vector<string> v;
		lsdir(dirpath, v);
	}
	cout << "\nDone." << dendl;
	cout << "Stopwatch shows: " << gsw.get() << dendl;
	cout << "Average time: " << ((double) gsw.get()) / DIR_SIZE << " us." << dendl;

	// simulate: rmdir
	gsw.reset_zero();
	for (int i = 0; i < DIR_SIZE; i++) {
		string dirpath = "/" + std::to_string(i);
		cout << "Removing directory " << dirpath << dendl;
		rmdir(dirpath);
	}
	cout << "\nDone." << dendl;
	cout << "Stopwatch shows: " << gsw.get() << dendl;
	cout << "Average time: " << ((double) gsw.get()) / DIR_SIZE << " us." << dendl;

	return true;
}

int main(int argc, char ** argv)
{
	g_conf.set("mds_md_dist_strategy", METADATA_STRATEGY);

	Cluster c;
	
	// add Monitor
	Host * mon_host = c.add_host("monitor");
	Service * mon_service = Service::create(mon_host, "mon");
	mon_service->start();
	Port mon_port = mon_service->get_port();

	dout << __func__ << " Registered monitor on " << mon_host->name() << " port " << mon_port << dendl;

	for (int i = 0; i < 1; i++) {
		Host * mds_host = c.add_host("mdshost." + std::to_string(i));
		for (int j = 0; j < 2; j++) {
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
