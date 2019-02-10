#ifndef CLOCK_H
#define CLOCK_H
#include <Windows.h>
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

	double	Now			() const;
	void	Set_Epoch	();
	void	Set_Epoch	(double in_epoch);
	void	Pause		();
	void	Resume		();
	bool	Is_Paused	() const;
};

#endif
