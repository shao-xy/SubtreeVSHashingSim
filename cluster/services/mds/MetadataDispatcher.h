#ifndef __CLUSTER_SERVICES_MDS_METADATADISPATCHER_H__
#define __CLUSTER_SERVICES_MDS_METADATADISPATCHER_H__

#include "include/types.h"

#include "cluster/services/mds/mdstypes.h"

class Host;
class MONService;

class MetadataDispatcher {
protected:
	MONService * mon;
	MetadataDispatcher(MONService * mon) : mon(mon) {}

	map<Host *, vector<MDSRank> *> lut;
	void build_lut();
	void clear_lut();

	int which_child(string path);
public:
	static MetadataDispatcher * create(string strategy, MONService * mon);
	~MetadataDispatcher() { clear_lut(); }

	virtual MDSRank dispatch(string path) = 0;
};

class SubtreeMetadataDispatcher : public MetadataDispatcher {
public:
	SubtreeMetadataDispatcher(MONService * mon);
	MDSRank dispatch(string path) override;
};

class HashingMetadataDispatcher : public MetadataDispatcher {
public:
	HashingMetadataDispatcher(MONService * mon);
	MDSRank dispatch(string path) override;
};

class HybridMetadataDispatcher : public MetadataDispatcher {
public:
	HybridMetadataDispatcher(MONService * mon);
	MDSRank dispatch(string path) override;
};

#endif /* cluster/services/mds/MetadataDispatcher.h */
