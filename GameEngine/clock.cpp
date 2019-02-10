#include "clock.h"

Clock::Clock() {
	std::lock_guard<std::mutex> lock(mMutex);
	mStartTime = SysClock::now();
	mPauseTime = SysTimePoint();
}

Clock::Clock(const Clock& in_source) :
mStartTime(in_source.mStartTime),
mPauseTime(in_source.mPauseTime)
{}

double Clock::Now() const {
	std::lock_guard<std::mutex> lock(mMutex);
	if (mPauseTime.time_since_epoch().count() != 0)
		return std::chrono::duration_cast<std::chrono::duration<double>>(mPauseTime - mStartTime).count();
	else
		return std::chrono::duration_cast<std::chrono::duration<double>>(SysClock::now() - mStartTime).count();
}

void Clock::Set_Epoch() {
	std::lock_guard<std::mutex> lock(mMutex);
	mStartTime = SysClock::now();
}

void Clock::Set_Epoch(double t) {
	std::lock_guard<std::mutex> lock(mMutex);
	mStartTime += std::chrono::duration_cast<ExactTime>(std::chrono::duration<double>(t));
}

void Clock::Pause() {
	std::lock_guard<std::mutex> lock(mMutex);
	mPauseTime = SysClock::now();
}

void Clock::Resume() {
	std::lock_guard<std::mutex> lock(mMutex);
	mStartTime += SysClock::now() - mPauseTime;
	mPauseTime = SysTimePoint();
}

bool Clock::Is_Paused() const {
	std::lock_guard<std::mutex> lock(mMutex);
	return mPauseTime == SysTimePoint();
}