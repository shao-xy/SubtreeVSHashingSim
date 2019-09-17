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
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);  
	}  
 
	return res;  
} 

string dirname(string path)
{
	size_t last_slash_pos = path.rfind('/');
	return last_slash_pos == string::npos ? "/" : path.substr(0, last_slash_pos);
}

string basename(string path)
{
	size_t last_slash_pos = path.rfind('/');
	return last_slash_pos == string::npos ? path : path.substr(last_slash_pos + 1);
}
