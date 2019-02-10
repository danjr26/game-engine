#ifndef CLOCK_H
#define CLOCK_H
#include <windows.h>
#include <chrono>
#include <mutex>

using SysClock = std::chrono::high_resolution_clock;
using SysTimePoint = std::chrono::time_point<SysClock>;
using ExactTime = SysClock::duration;

class Clock {
private:
	mutable std::mutex mMutex;
	SysTimePoint mStartTime;
	SysTimePoint mPauseTime;

public:
	Clock();
	Clock(const Clock& in_source);

	double	now			() const;
	void	setEpoch	();
	void	setEpoch	(double in_epoch);
	void	pause		();
	void	resume		();
	bool	isPaused	() const;
};

#endif
