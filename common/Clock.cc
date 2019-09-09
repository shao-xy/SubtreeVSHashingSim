#include <sys/time.h>

#include <sstream>

#include "Clock.h"

std::string clock_now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm * local = localtime(&tv.tv_sec);
	std::stringstream ss;
	ss << (1900 + local->tm_year) << '/' << (1 + local->tm_mon) << '/' << local->tm_mday << ' ' << local->tm_hour << ':' << local->tm_min << ':' << local->tm_sec << ':' << tv.tv_usec;
	return ss.str();
}
