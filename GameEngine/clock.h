#ifndef CLOCK_H
#define CLOCK_H
#include <Windows.h>
#include <chrono>
#include "component.h"

using SysClock =		std::chrono::high_resolution_clock;
using SysTimePoint =	std::chrono::time_point<SysClock>;
using ExactTime =		SysClock::duration;

class Clock : public MutexProtected {
private:
	SysTimePoint startTime;
	SysTimePoint pauseTime;

public:
	Clock();

	double	Now			();
	void	Set_Epoch	();
	void	Set_Epoch	(double in_epoch);
	void	Pause		();
	void	Resume		();
	bool	Is_Paused	();
};

#endif
