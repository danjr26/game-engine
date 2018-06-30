#ifndef LOG_H
#define LOG_H

#include <Windows.h>
#include <string>
#include <mutex>

class Log {
public:
	static Log main;

	mutable std::mutex mutex;

	Log();
	Log(const Log& in_source);
	~Log();

	void operator()(std::string s) const;
};

#endif