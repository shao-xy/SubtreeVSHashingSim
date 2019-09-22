#ifndef __CLUSTER_PROCESS_CLIENTPROCESS_H__
#define __CLUSTER_PROCESS_CLIENTPROCESS_H__

#include "include/types.h"
#include "include/LRU.h"

#include "cluster/Process.h"

#include "cluster/messages/MClientRequest.h"

#include "cluster/network/NetworkEntity.h"

#define CLIENT_LRU_SIZE 1000

class CInode;

class ClientProcess : public Process {
private:
	NetworkEntity mon;
	NetworkEntity root_mds;
	bool connected;

	CInode * callback_inoptr;
	vector<CInode *> callback_inolist;

	LRUCache<string, CInode *> cache;
public:
	ClientProcess(Host * h) : Process(h), connected(false), cache(CLIENT_LRU_SIZE) {}
	ClientProcess(Host * h, Host * mon_host, Port mon_p) : Process(h), mon(mon_host, mon_p), connected(false), cache(CLIENT_LRU_SIZE) {}
	~ClientProcess() {}

	virtual bool handle_message(Message * m) override;
private:
	bool handle_findmdsack(Message * m);
	bool handle_clientrequestreply(Message * m);
public:
	bool connect_cluster();
private:
	bool send_request(string fullpath, msg_op_fs_t op, CInode * ino, string data = "");
	bool send_request(string fullpath, msg_op_fs_t op, string path = "", CInode * ino = NULL, string data = "");
private:
	CInode * _lookup(string path);
	CInode * lookup(string path);
	CInode * local_lookup(string path);
	bool remote_lookup(string path);
public:
	bool mknod(string path);
	bool rm(string path);

	bool read(string path, string & out);
	bool write(string path, string & in);

	bool mkdir(string path);
	bool rmdir(string path);
private:
	bool _lsdir(string path);
public:
	bool lsdir(string path, vector<string> & ret);
	bool lsdir(string path, vector<CInode *> & ret);

	bool exist(string path);
	bool is_file(string path);
	bool is_dir(string path);
};

#endif /* cluster/process/ClientProcess.h */
