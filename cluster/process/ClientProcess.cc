#include "ClientProcess.h"

#include "common/Debug.h"

#include "cluster/Host.h"

#include "cluster/messages/MFindMDS.h"
#include "cluster/messages/MFindMDSAck.h"

#undef dout_prefix
#define dout_prefix get_host()->name() << ' '

bool ClientProcess::handle_message(Message * m)
{
	if (!m)	return false;

	bool ret = false;
	switch (m->get_type())
	{
		case MSG_FINDMDSACK:
			ret = handle_findmdsack(m);
			break;
		default:
			break;
	}

	delete m;
	return ret;
}

bool ClientProcess::handle_findmdsack(Message * m)
{
	MFindMDSAck * msg = static_cast<MFindMDSAck *>(m);
	root_mds.host = msg->root_mds.host;
	root_mds.port = msg->root_mds.port;

	//dout << __func__ << " Get known: root MDS at " << root_mds.host->name() << " port " << root_mds.port << dendl;
	dout << __func__ << " Get known: root MDS at " << root_mds << dendl;
	return true;
}

bool ClientProcess::connect_cluster()
{
	dout << __func__ << " Trying to connect to cluster" << dendl;

	if (connected)	return true;

	connected = send_message(&mon, new MFindMDS());

	return connected;
}
