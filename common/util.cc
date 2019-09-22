#include <cstring>

#include "util.h"

vector<unsigned int> findAll(const string & str, const string & delim)
{
	vector<unsigned int> ret;
	
	for (unsigned int index = 0; (index = str.find(delim, index)) < str.length(); index++) {
		ret.push_back(index);
	}

	return ret;
}

vector<string> split(const string & str, const string & delim)
{
	vector<string> res;  
	if("" == str) return res;  

	char * strs = new char[str.length() + 1];
	strcpy(strs, str.c_str());   
 
	char * d = new char[delim.length() + 1];  
	strcpy(d, delim.c_str());  
 
	char *p = strtok(strs, d);  
	while(p) {  
		string s = p;
		res.push_back(s);
		p = strtok(NULL, d);  
	}  
 
	return res;  
} 

vector<string> split_path(const string & str, const string & delim)
{
	vector<string> res;  
	if("" == str) return res;  

	char * strs = new char[str.length() + 1];
	strcpy(strs, str.c_str());   
 
	char * d = new char[delim.length() + 1];  
	strcpy(d, delim.c_str());  
 
	char *p = strtok(strs, d);  
	while(p) {  
		string s = p;
		if (s == "..") {
			if (!res.empty()) res.pop_back();
		}
		else if (s != "" && s != ".")
			res.push_back(s);
		p = strtok(NULL, d);  
	}  
 
	return res;  
}

bool filename_valid(string path)
{
	return path.find('/') == string::npos;
}

string dirname(string path)
{
	if (path.back() == '/') path = path.substr(0, path.length() - 1);

	size_t last_slash_pos = path.rfind('/');
	if (last_slash_pos == string::npos)	return "";
	
	string dirpath = path.substr(0, last_slash_pos);
	return dirpath == "" ? "/" : dirpath;
}

string basename(string path)
{
	if (path.back() == '/') path = path.substr(0, path.length() - 1);
	
	size_t last_slash_pos = path.rfind('/');
	return last_slash_pos == string::npos ? path : path.substr(last_slash_pos + 1);
}

size_t hash(string s, size_t divisor)
{
	if (!divisor)	return 0;
	return std::hash<string>()(s) % divisor;
}
