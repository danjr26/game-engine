#ifndef LOG_H
#define LOG_H

#include <windows.h>
#include <string>
#include <mutex>
#include "exceptions.h"

class Log {
public:
	static Log main;

	mutable std::mutex mMutex;

	Log();
	Log(const Log& in_source);
	~Log();

	void operator()(std::string s) const;
};

#endif