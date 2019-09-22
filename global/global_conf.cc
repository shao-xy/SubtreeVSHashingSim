#include "global_conf.h"

const map<string, string> GlobalConf::_CONF_DEFAULT = {_conf_vtype("mds_md_dist_strategy", "subtree")};
const vector<string> GlobalConf::CONF_KEYS = {"mds_md_dist_strategy"};

GlobalConf g_conf;

GlobalConf::GlobalConf()
{
	for (auto it = _CONF_DEFAULT.begin(); it != _CONF_DEFAULT.end(); it++)
		_confs.insert(*it);
}

string GlobalConf::get(string key)
{
	if (!key_valid(key))	return "";
	return _confs[key];
}

bool GlobalConf::set(string key, string value)
{
	if (!key_valid(key))	return false;
	_confs[key] = value;
	return true;
}
