#include "frame_rate_manager.h"
#include "misc.h"
#include "log.h"
#include <thread>

FrameRateManager::FrameRateManager(double in_fps) :
	mTimeStepper(1.0 / in_fps),
	mNTrackedFrames(60),
	mNBackedUp(0),
	mMaxBackedUp(6) {

	mLastFrameTimes.reserve(mNTrackedFrames);

	mLastFrame = mClock.Now();
}

void FrameRateManager::Set_FPS(double in_fps) {
	mTimeStepper.mStep = 1.0 / in_fps;
}

double FrameRateManager::Get_FPS() const {
	return 1.0 / mTimeStepper.mStep;
}

void FrameRateManager::Set_Dt(double in_dt) {
	mTimeStepper.mStep = in_dt;
}

double FrameRateManager::Get_Dt() const {
	return mTimeStepper.mStep;
}

void FrameRateManager::Reset_Timer() {
	mLastFrame = mClock.Now();
}

void FrameRateManager::Yield_Until_Next_Frame() {
	double now = mClock.Now();
	uint nFrames = mTimeStepper.Step_Number(now - mLastFrame) + mNBackedUp;

	while (nFrames == 0) {
		now = mClock.Now();
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		nFrames = mTimeStepper.Step_Number(mClock.Now() - now);
	}
	mNBackedUp = nFrames - 1;

	now = mClock.Now();
	double thisFrameTime = now - mLastFrame;
	mLastFrame = now;

	if (mLastFrameTimes.size() < mNTrackedFrames) {
		mLastFrameTimes.push_back(thisFrameTime);
	}
	else {
		for (uint i = 0; i < mLastFrameTimes.size() - 1; i++) {
			mLastFrameTimes[i] = mLastFrameTimes[i + 1];
		}
		mLastFrameTimes[mLastFrameTimes.size() - 1] = thisFrameTime;
	}
}

bool FrameRateManager::Is_Lean_Frame() const {
	return mNBackedUp >= mMaxBackedUp;
}

double FrameRateManager::Get_Real_FPS() {
	if (mLastFrameTimes.size() == 0) {
		return 0.0;
	}
	double total = 0.0;
	for (uint i = 0; i < mLastFrameTimes.size(); i++) {
		total += mLastFrameTimes[i];
	}
	return mLastFrameTimes.size() / total;
}

double FrameRateManager::Get_Longest_Frame_Time() {
	if (mLastFrameTimes.size() == 0) {
		return 0.0;
	}
	return Max(mLastFrameTimes);
}

double FrameRateManager::Get_Last_Frame_Time() {
	if (mLastFrameTimes.size() == 0) {
		return 0.0;
	}
	return mLastFrameTimes[mLastFrameTimes.size() - 1];
}
