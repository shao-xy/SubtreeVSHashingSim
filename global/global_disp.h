#ifndef __GLOBAL_GLOBAL_DISP_H__
#define __GLOBAL_GLOBAL_DISP_H__

#include "cluster/services/mds/MetadataDispatcher.h"

extern MetadataDispatcher * subtreeDisp;
extern MetadataDispatcher * hashingDisp;
extern MetadataDispatcher * hybridDisp;

void global_create_dispatchers(MONService * mon);
MetadataDispatcher * global_get_dispatcher(string type);
MetadataDispatcher * global_get_dispatcher_from_conf();

#endif /* global/global_disp.h */
