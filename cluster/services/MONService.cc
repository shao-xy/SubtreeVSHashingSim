#include "MONService.h"

#include "common/Debug.h"

#include "cluster/Host.h"

#include "cluster/messages/MFindMDSAck.h"

#undef dout_prefix
#define dout_prefix get_host()->name() << "::MONService "

bool MONService::entry()
{
	return true;
}

bool MONService::handle_message(Message * m)
{
	if (!m)	return false;

	int type = m->get_type();

	bool ret = false;
	
	switch (type) {
		case MSG_FINDMDS:
			ret = handle_findmds(m);
		default: break;
	};
	
	delete m;
	return ret;
}

bool MONService::handle_findmds(Message * m)
{
	dout << __func__ << " Get find mds request from " << m->src << dendl;
	MFindMDSAck * mack = new MFindMDSAck();
	//mack->host = 
	return send_message(&m->src, mack);
}
