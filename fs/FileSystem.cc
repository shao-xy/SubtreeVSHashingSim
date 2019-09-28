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
	for (vector<Inode *>::iterator it = entries.begin(); it != entries.end(); ) {
		if (*it == inode)
			it = entries.erase(it);
		else
			it++;
	}
}

void FileSystem::Inode::delete_sub(string name)
{
	for (vector<Inode *>::iterator it = entries.begin(); it != entries.end(); ) {
		if ((*it)->name == name)
			it = entries.erase(it);
		else
			it++;
	}
}

void FileSystem::Inode::delete_myself()
{
	if (!parent)	return;
	delete_myself_recur();
	parent->delete_sub(this);
	delete this;
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

bool FileSystem::is_dir(string path)
{
	Inode * inode = lookup(path);
	return inode && inode->is_dir();
}

FileSystem::Inode * FileSystem::mknod(FileSystem::Inode * parent, string name)
{
	if (!parent || !::filename_valid(name))	return NULL;
	Inode * ino = new Inode(name, false, parent);
	parent->add_sub(ino);
	return ino;
}

bool FileSystem::rm(FileSystem::Inode * ino)
{
	if (!ino)	return false;
	ino->delete_myself();
	return true;
}

bool FileSystem::read(FileSystem::Inode * ino, string & out)
{
	if (!ino)	return false;
	out = ino->get_content();
	return true;
}

bool FileSystem::write(FileSystem::Inode * ino, string & in)
{
	if (!ino)	return false;
	ino->set_content(in);
	return true;
}

FileSystem::Inode * FileSystem::mkdir(FileSystem::Inode * parent, string name)
{
	if (!parent || !::filename_valid(name))	return NULL;
	if (parent->get_sub(name))	return NULL;

	Inode * ino = new Inode(name, true, parent);
	parent->add_sub(ino);
	return ino;
}

bool FileSystem::rmdir(FileSystem::Inode * ino)
{
	if (!ino)	return false;
	ino->delete_myself();
	return true;
}

bool FileSystem::listdir(FileSystem::Inode * parent, vector<FileSystem::Inode *> & ret)
{
	if (!parent || !parent->is_dir())	return false;
	ret.clear();
	for (auto it = parent->begin(); it != parent->end(); it++) {
		ret.push_back(*it);
	}
	return true;
}

ostream & operator<<(ostream & os, Inode & inode)
{
	os << "[Inode " << &inode << " name = \"" << inode.get_name() << "\", isDir = " << inode.is_dir() << ", content = \"" << inode.get_content() << "\"]";
	return os;
}
