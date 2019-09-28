#include "MetadataDispatcher.h"

#include "common/util.h"

#include "cluster/services/MONService.h"

#include "fs/FileSystem.h"

MetadataDispatcher * MetadataDispatcher::create(string strategy, MONService * mon)
{
	MetadataDispatcher * disp = 0;
	if (strategy == "subtree")
		disp = new SubtreeMetadataDispatcher(mon);
	else if (strategy == "hashing")
		disp = new HashingMetadataDispatcher(mon);
	else if (strategy == "hybrid")
		disp = new HybridMetadataDispatcher(mon);
	
	return disp;
}

void MetadataDispatcher::build_lut()
{
	if (!mon)	return;

	clear_lut();
	for (auto it = mon->mds_begin(); it != mon->mds_end(); it++) {
		MDSRank r = it->first;
		Host * host = it->second->host;

		if (lut.count(host) == 0) {
			lut.insert(std::make_pair(host, new vector<MDSRank>()));
		}

		lut[host]->push_back(r);
	}
}

void MetadataDispatcher::clear_lut()
{
	for (auto it = lut.begin(); it != lut.end(); it++) {
		vector<MDSRank> * pv = it->second;
		if (pv)	delete pv;
	}
	lut.clear();
}

int MetadataDispatcher::which_child(string path)
{
	Inode * parent = gfs.lookup(::dirname(path));
	if (!parent)	return -1;

	string name = ::basename(path);
	for (auto it = parent->begin(); it != parent->end(); it++) {
		if ((*it)->get_name() == name)
			return it - parent->begin();
	}
	return -1;
}

SubtreeMetadataDispatcher::SubtreeMetadataDispatcher(MONService * mon) : MetadataDispatcher(mon) { build_lut(); }

HashingMetadataDispatcher::HashingMetadataDispatcher(MONService * mon) : MetadataDispatcher(mon) {}

HybridMetadataDispatcher::HybridMetadataDispatcher(MONService * mon) : MetadataDispatcher(mon) { build_lut(); }

MDSRank SubtreeMetadataDispatcher::dispatch(string path)
{
	if (!mon || path == "")	return -1;

	if (!gfs.is_dir(path))	return dispatch(::dirname(path));
	
	vector<string> v = split_path(path);
	MDSRank retrank = -1;
	switch (v.size()) {
		case 0:
			retrank = 0;	break;
		case 1: {
			int rank = which_child(path);
			auto it = lut.begin();
			for (unsigned int i = 0; i < (rank % lut.size()); i++, it++);
			retrank = (rank != -1) ? it->second->at(0) : -1;
			break;
		}
		default: {
			string level1_path = "/" + v[0];
			int parent_rank = which_child(level1_path);
			string level2_path = "/" + v[0] + "/" + v[1];
			int cur_rank = which_child(level2_path);
			if (parent_rank != -1 && cur_rank != -1) {
				auto it = lut.begin();
				for (unsigned int i = 0; i < (parent_rank % lut.size()); i++, it++);
				vector<MDSRank> * v = it->second;
				retrank = v->size() != 0 ? v->at(cur_rank % v->size()) : -1;
			}
			break;
		}
	}
	return retrank;
}

MDSRank HashingMetadataDispatcher::dispatch(string path)
{
	if (!mon || path == "")	return -1;
	size_t size = mon->mds_active_size();
	return size != 0 ? hash(path, size) : -1;
}

MDSRank HybridMetadataDispatcher::dispatch(string path)
{
	if (!mon || path == "")	return -1;
	
	vector<string> v = split_path(path);
	MDSRank retrank = -1;
	switch (v.size()) {
		case 0:
			retrank = 0;	break;
		case 1: {
			int rank = which_child(path);
			auto it = lut.begin();
			for (unsigned int i = 0; i < (rank % lut.size()); i++, it++);
			retrank = (rank != -1) ? it->second->at(0) : -1;
			break;
		}
		default: {
			string level1_path = "/" + v[0];
			int parent_rank = which_child(level1_path);
			if (parent_rank == -1)	break;
				
			auto it = lut.begin();
			for (unsigned int i = 0; i < (parent_rank % lut.size()); i++, it++);
			vector<MDSRank> * v = it->second;
			size_t size = v->size();
			retrank = size != 0 ? v->at(hash(path, size)) : -1;
			break;
		}
	}
	return retrank;
}
