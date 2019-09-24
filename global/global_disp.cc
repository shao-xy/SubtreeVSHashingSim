#include "global_disp.h"

#include "global_conf.h"

MetadataDispatcher * subtreeDisp = 0;
MetadataDispatcher * hashingDisp = 0;
MetadataDispatcher * hybridDisp = 0;

void global_create_dispatchers(MONService * mon)
{
	subtreeDisp = MetadataDispatcher::create("subtree", mon);
	hashingDisp = MetadataDispatcher::create("hashing", mon);
	hybridDisp = MetadataDispatcher::create("hybrid", mon);
}

MetadataDispatcher * global_get_dispatcher(string type)
{
	MetadataDispatcher * disp = 0;
	if (type == "subtree")
		disp = subtreeDisp;
	else if (type == "hashing")
		disp = hashingDisp;
	else if (type == "hybrid")
		disp = hybridDisp;

	return disp;
}

MetadataDispatcher * global_get_dispatcher_from_conf()
{
	return global_get_dispatcher(g_conf.get("mds_md_dist_strategy"));
}
