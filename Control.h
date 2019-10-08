#ifndef __CONTROL_H__
#define __CONTROL_H__

// Host num and MDS size per host
#define HOST_SIZE 5
#define MDS_SIZE_PER_HOST 5

#define DIR_SIZE 10
#define FILE_SIZE_PER_DIR 10000
#define FILE_SIZE (DIR_SIZE * FILE_SIZE_PER_DIR)

#define METADATA_STRATEGY "hashing"

// Network type
#define NETWORK_LAT_SAME_HOST "ipc"
#define NETWORK_LAT_OTHER_HOST "tcp"

// Client Cache size
#define ENABLE_CLIENT_CACHE 1
#define CLIENT_LRU_SIZE 50000

#endif /* Control.h */
