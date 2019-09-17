#include "fs/FileSystem.h"

#include "common/util.h"

FileSystem gfs;

FileSystem::Inode * FileSystem::Inode::get_sub(string subpath)
{
	vector<Inode *>::iterator it = get_subitr(subpath);
	return it == entries.end() ? NULL : (*it);
}

vector<FileSystem::Inode *>::iterator FileSystem::Inode::get_subitr(string subpath)
{
	for (auto it = entries.begin(); it != entries.end(); it++) {
		if ((*it)->get_name() == subpath)	return it;
	}
	return entries.end();
}

void FileSystem::Inode::delete_sub(FileSystem::Inode * inode)
{
	for (vector<Inode *>::iterator it = entries.begin(); it != entries.end(); it++) {
		if (*it == inode)	entries.erase(it);
	}
}

void FileSystem::Inode::delete_sub(string name)
{
	for (vector<Inode *>::iterator it = entries.begin(); it != entries.end(); it++) {
		if ((*it)->name == name)	entries.erase(it);
	}
}

void FileSystem::Inode::delete_myself()
{
	parent->delete_sub(this);
	delete_myself_recur();
}

void FileSystem::Inode::delete_myself_recur()
{
	if (!isDir) return;

	for (vector<Inode *>::iterator it = entries.begin(); it != entries.end();) {
		(*it)->delete_myself_recur();
		it = entries.erase(it);
	}
}

FileSystem::Inode * FileSystem::lookup(string path, Inode * start)
{
	if (path == "")	return NULL;
	if (!start)	start = root;

	vector<string> path_list = ::split(path, "/");
	Inode * inode = start;
	vector<Inode *>::iterator it;
	for (string subpath : path_list) {
		it = inode->get_subitr(subpath);
		if (it == inode->end())	return NULL;
		inode = *it;
	}
	return inode;
}

bool FileSystem::mknod(string path)
{
	string parent_path = dirname(path);
	string name = basename(path);
	Inode * parent = lookup(parent_path);
	if (!parent)	return false;
	parent->add_sub(new Inode(name, false));
	return true;
}

bool FileSystem::rm(string path)
{
	string parent_path = dirname(path);
	string name = basename(path);
	Inode * parent = lookup(parent_path);
	if (!parent)	return false;
	parent->delete_sub(name);
	return true;
}

bool FileSystem::read(string path, string & out)
{
	Inode * inode = lookup(path);
	if (!inode)	return false;

	out = inode->get_content();
	return true;
}

bool FileSystem::write(string path, string & in)
{
	Inode * inode = lookup(path);
	if (!inode)	return false;

	inode->set_content(in);
	return true;
}

bool FileSystem::mkdir(string path)
{
	string parent_path = dirname(path);
	string name = basename(path);
	Inode * parent = lookup(parent_path);
	if (!parent)	return false;
	parent->add_sub(new Inode(name, true));
	return true;
}

bool FileSystem::rmdir(string path)
{
	Inode * inode = lookup(path);
	if (!inode)	return false;

	inode->delete_myself();
	return true;
}

