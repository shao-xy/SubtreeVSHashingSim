#ifndef __FS_FILESYSTEM_H__
#define __FS_FILESYSTEM_H__

#include "include/types.h"

class FileSystem {
public:
	class Inode {
	public:
		string name;
		bool isDir;

		Inode * parent;
		vector<Inode *> entries;

		string content;

		Inode(string name, bool isDir) : name(name), isDir(isDir) {}

		string get_name() { return name; }
		void set_name(string newname) { name = newname; }
		string get_content() { return content; }
		void set_content(string newcontent) { content = newcontent; }

		Inode * get_parent() { return parent; }
		void set_parent(Inode * p) { parent = p; }
		vector<Inode *>::iterator begin() { return entries.begin(); }
		vector<Inode *>::iterator end() { return entries.end(); }

		Inode * get_sub(string subpath);
		vector<Inode *>::iterator get_subitr(string subpath);
		void add_sub(Inode * inode) { if (inode)	entries.push_back(inode); }
		void delete_sub(vector<Inode *>::iterator it_inode) { entries.erase(it_inode); }
		void delete_sub(Inode * inode);
		void delete_sub(string name);

		void delete_myself();
	private:
		void delete_myself_recur();
	};
private:
	Inode * root;
public:
	FileSystem() : root(new Inode("/", true)) {}

	Inode * lookup(string path, Inode * start = NULL);

	bool mknod(string path);
	bool rm(string path);

	bool read(string path, string & out);
	bool write(string path, string & in);

	bool mkdir(string path);
	bool rmdir(string path);
};

extern FileSystem gfs;

#endif /* fs/FileSystem.h */
