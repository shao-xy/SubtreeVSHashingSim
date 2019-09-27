#ifndef __FS_FILESYSTEM_H__
#define __FS_FILESYSTEM_H__

#include <ostream>
using std::ostream;

#include "include/types.h"

class FileSystem {
public:
	class Inode {
		string name;
		bool isDir;

		Inode * parent;
		vector<Inode *> entries;

		string content;

	public:
		Inode(string name, bool isDir, Inode * parent = NULL) : name(name), isDir(isDir), parent(parent) {}

		bool is_file() { return !isDir; }
		bool is_dir() { return isDir; }

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

	Inode * mknod(Inode * parent, string name);
	bool rm(Inode * ino);

	bool read(Inode * ino, string & out);
	bool write(Inode * ino, string & in);

	Inode * mkdir(Inode * parent, string name);
	bool rmdir(Inode * ino);
	bool listdir(Inode * parent, vector<Inode *> & ret);
};

typedef FileSystem::Inode Inode;

ostream & operator<<(ostream & os, Inode & inode);

extern FileSystem gfs;

#endif /* fs/FileSystem.h */
