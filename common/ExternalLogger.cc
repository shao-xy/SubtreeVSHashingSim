#include <vector>

#include "ExternalLogger.h"

#include "Clock.h"
#include "util.h"

using std::vector;

ExternalLogger gdout;

void ExternalLogger::init_ofs(string path)
{
	this->ofs.open(path, ios::out|ios::app);
	if (!this->ofs.is_open())	throw "[UTIL] External Logger failed to open log file";
}

bool ExternalLogger::line(char tag, string title, string message)
{
	title.resize(6, ' ');

	vector<string> msg_lines = ::split(message, "\n");

	for (size_t i = 0; i < msg_lines.size(); i++)
		ofs << clock_now() << ' ' << tag << " [" << title << "]\t" << msg_lines[i] << '\n';
	ofs.flush();

	return true;
}

bool ExternalLogger::set_path(string path)
{
	ofs.close();
	opath = path;
	init_ofs(path);
	return true;
}

bool ExternalLogger::flush()
{
	return ofs.flush();
}

bool ExternalLogger::v(string title, string message)
{
	return line('V', title, message);
}

bool ExternalLogger::i(string title, string message)
{
	return line('I', title, message);
}

bool ExternalLogger::w(string title, string message)
{
	return line('W', title, message);
}

bool ExternalLogger::e(string title, string message)
{
	return line('E', title, message);
}

ExternalLogger & ExternalLogger::operator<<(manip manipulator)
{
	ofs << manipulator;
	flush();
	return *this;
}
