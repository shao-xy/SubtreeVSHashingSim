#include "common/Debug.h"

#include "fs/FileSystem.h"

typedef FileSystem::Inode Inode;

int main()
{
	gfs.mkdir("/a/");
	gfs.mknod("/a/../b");
	gfs.mknod("/a/.//../a/c");

	vector<string> v;
	gfs.lsdir("/", v);

	for (string s : v) {
		dout << s << dendl;
	}
}
