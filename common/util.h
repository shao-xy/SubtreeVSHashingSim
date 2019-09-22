#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__

#include <include/types.h>

vector<unsigned int> findAll(const string & str, const string & delim);
vector<string> split(const string & str, const string & delim);
vector<string> split_path(const string & str, const string & delim = "/");

bool filename_valid(string path);

string dirname(string path);
string basename(string path);

size_t hash(string s, size_t divisor = 1);

#endif
