#include "common/Debug.h"

#include "fs/FileSystem.h"

int main()
{
	Inode * root = gfs.lookup("/");
	Inode * dir_a = gfs.mkdir(root, "a");
	gfs.mknod(dir_a, "b");
	gfs.mknod(root, "c");

	vector<Inode *> v;
	gfs.listdir(root, v);

	for (Inode * ino : v) {
		dout << ino->get_name() << dendl;
	}
}
