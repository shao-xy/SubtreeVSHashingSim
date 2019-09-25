#include "common/ExternalLogger.h"

using std::ostream;

class TestClass {
public:
	int a;
	TestClass(int a) : a(a) {}
};

ostream & operator<<(ostream & os, TestClass t)
{
	os << "[TestClass a = " << t.a;
	return os;
}

int main()
{
	//gdout << "Test" << std::endl;
	gdout << TestClass(1) << std::endl;
	return 0;
}
