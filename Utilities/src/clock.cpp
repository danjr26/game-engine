#include "../include/internal/clock.h"

Clock::Clock() :
mStartTime(clock_t::now()),
mPauseTime()
{}

double Clock::now() const {
	time_point_t t0 = (isPaused()) ? mPauseTime : mStartTime;
	time_point_t t1 = clock_t::now();
	return std::chrono::duration<double>(t1 - t0).count();
}

ullong Clock::nowSec() const {
	return std::chrono::duration_cast<std::chrono::seconds>(nowExact()).count();
}

ullong Clock::nowMilli() const {
	return std::chrono::duration_cast<std::chrono::milliseconds>(nowExact()).count();
}

ullong Clock::nowMicro() const {
	return std::chrono::duration_cast<std::chrono::microseconds>(nowExact()).count();
}

ullong Clock::nowNano() const {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(nowExact()).count();
}

Clock::duration_t Clock::nowExact() const {
	time_point_t t0 = (isPaused()) ? mPauseTime : mStartTime;
	time_point_t t1 = clock_t::now();
	return t1 - t0;
}