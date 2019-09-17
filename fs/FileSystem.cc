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

	for (vector<Inode *>::iterator it = entries.begin(); it != entries.end(); it = entries.erase(it)) {
		(*it)->delete_myself_recur();
	}
}

FileSystem::Inode * FileSystem::lookup(string path, Inode * start)
{
	if (path == "")	return NULL;
	if (!start)	start = root;

	vector<string> path_list = ::split_path(path, "/");
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
	if (path.back() == '/')	path = path.substr(0, path.size() -1);
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

bool FileSystem::lsdir(string path, vector<string> & ret)
{
	Inode * inode = lookup(path);
	if (!inode || !inode->is_dir())	return false;

	ret.clear();
	for (auto it = inode->begin(); it != inode->end(); it++) {
		ret.push_back((*it)->get_name());
	}
	return true;
}

bool FileSystem::lsdir(string path, vector<FileSystem::Inode *> & ret)
{
	Inode * inode = lookup(path);
	if (!inode || !inode->is_dir())	return false;

	ret.clear();
	for (auto it = inode->begin(); it != inode->end(); it++) {
		ret.push_back((*it));
	}
	return true;
}

bool FileSystem::exist(string path)
{
	Inode * inode = lookup(path);
	return inode != NULL;
}

bool FileSystem::is_file(string path)
{
	Inode * inode = lookup(path);
	return inode && !inode->is_dir();
}

bool FileSystem::is_dir(string path)
{
	Inode * inode = lookup(path);
	return inode && inode->is_dir();
}
