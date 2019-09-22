#ifndef __FS_CACHEDOBJECT_H__
#define __FS_CACHEDOBJECT_H__

#include "FileSystem.h"

class CInode {
	Inode * ino;
public:
	CInode(Inode * ino = NULL) : ino(ino) {}
	Inode * get() { return ino; }

	string get_name() { return ino->get_name(); }

	bool is_file() { return ino->is_file(); }
	bool is_dir() { return ino->is_dir(); }
};

#endif /* fs/CachedObject.h */
