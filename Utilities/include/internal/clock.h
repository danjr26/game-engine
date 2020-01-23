#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

#include "definitions.h"

class Clock {
public:
	using clock_t = std::chrono::high_resolution_clock;
	using duration_t = clock_t::duration;
	using time_point_t = std::chrono::time_point<clock_t>;

private:
	time_point_t mStartTime;
	time_point_t mPauseTime;

public:
	Clock();

	double now() const;
	ullong nowSec() const;
	ullong nowMilli() const;
	ullong nowMicro() const;
	ullong nowNano() const;
	duration_t nowExact() const;
	
	void reset();
	void pause();
	void unpause();
	bool isPaused() const;
};

#endif
