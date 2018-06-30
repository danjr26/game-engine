#include "log.h"

Log Log::main = Log();


Log::Log() {
	(*this)("\n\n-------- START LOG --------\n");
}

Log::Log(const Log& in_source) 
{}

Log::~Log() {
	(*this)("\n-------- END LOG --------\n\n");
}

void Log::operator()(std::string s) const {
	std::lock_guard<std::mutex> lock(mutex);
	OutputDebugStringA((s + "\n").c_str());
}