#include "clock.h"

Clock::Clock() {
	std::lock_guard<std::mutex> lock(mutex);
	startTime = SysClock::now();
	pauseTime = SysTimePoint();
}

double Clock::Now() {
	std::lock_guard<std::mutex> lock(mutex);
	if (pauseTime.time_since_epoch().count() != 0)
		return std::chrono::duration_cast<std::chrono::duration<double>>(pauseTime - startTime).count();
	else
		return std::chrono::duration_cast<std::chrono::duration<double>>(SysClock::now() - startTime).count();
}

void Clock::Set_Epoch() {
	std::lock_guard<std::mutex> lock(mutex);
	startTime = SysClock::now();
}

void Clock::Set_Epoch(double t) {
	std::lock_guard<std::mutex> lock(mutex);
	startTime += std::chrono::duration_cast<ExactTime>(std::chrono::duration<double>(t));
}

void Clock::Pause() {
	std::lock_guard<std::mutex> lock(mutex);
	pauseTime = SysClock::now();
}

void Clock::Resume() {
	std::lock_guard<std::mutex> lock(mutex);
	startTime += SysClock::now() - pauseTime;
	pauseTime = SysTimePoint();
}

bool Clock::Is_Paused() {
	std::lock_guard<std::mutex> lock(mutex);
	return pauseTime == SysTimePoint();
}