#ifndef __GLOBAL_GLOBAL_CONF_H__
#define __GLOBAL_GLOBAL_CONF_H__

#include <algorithm>

#include "include/types.h"

class GlobalConf {
private:
	using _conf_vtype = map<string, string>::value_type;
	map<string, string> _confs;
	static const map<string, string> _CONF_DEFAULT;
public:
	static const vector<string> CONF_KEYS;

	bool key_valid(string key) { return std::find(CONF_KEYS.begin(), CONF_KEYS.end(), key) != CONF_KEYS.end(); }

	GlobalConf();

	string get(string key);
	bool set(string key, string value);
};

extern GlobalConf g_conf;

#endif /* global/global_conf.h */
