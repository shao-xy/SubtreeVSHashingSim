#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__

#include <include/types.h>

vector<unsigned int> findAll(const string & str, const string & delim);
vector<string> split(const string & str, const string & delim);

string dirname(string path);
string basename(string path);

#endif
