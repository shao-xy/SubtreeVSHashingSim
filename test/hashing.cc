#include <iostream>
#include <string>

#include "common/util.h"

using namespace std;

int main()
{
	cout << std::hash<string>()("test") << endl;
	cout << ::hash("test") << endl;
	return 0;
}
