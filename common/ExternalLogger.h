#ifndef __EXTERNAL_LOGGER__
#define __EXTERNAL_LOGGER__

#include <fstream>

using std::string;
using std::ios;
using std::ofstream;

typedef std::ostream& (*manip)(std::ostream&);
#define DEFAULT_LOGGER_FILE "Simulator.log"

class ExternalLogger : public std::ostream {
private:
	string opath;
	ofstream ofs;
	void init_ofs(string path);
	bool line(char tag, string title, string message);
public:
	ExternalLogger(string path = DEFAULT_LOGGER_FILE)
		: opath(path)
		{
		init_ofs(path);
	}
	~ExternalLogger() {
		if (ofs.is_open()) {
			ofs.close();
		}
	}
	string get_path() {
		return opath;
	}
	bool set_path(string path);
	bool flush();
	bool v(string title, string message);
	bool i(string title, string message);
	bool w(string title, string message);
	bool e(string title, string message);


	template <typename T>
	ExternalLogger & operator<<(T const& obj) override {
		ofs << obj;
		flush();
		return *this;
	}
	ExternalLogger & operator<<(manip manipulator);
};

extern ExternalLogger gdout;

#endif /* ExternalLogger.h */
