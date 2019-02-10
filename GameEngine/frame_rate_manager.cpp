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

	mLastFrame = mClock.now();
}

void FrameRateManager::setFPS(double in_fps) {
	mTimeStepper.mStep = 1.0 / in_fps;
}

double FrameRateManager::getFPS() const {
	return 1.0 / mTimeStepper.mStep;
}

void FrameRateManager::setDt(double in_dt) {
	mTimeStepper.mStep = in_dt;
}

double FrameRateManager::getDt() const {
	return mTimeStepper.mStep;
}

void FrameRateManager::resetTimer() {
	mLastFrame = mClock.now();
}

void FrameRateManager::yieldUntilNextFrame() {
	double now = mClock.now();
	uint nFrames = mTimeStepper.stepNumber(now - mLastFrame) + mNBackedUp;

	while (nFrames == 0) {
		now = mClock.now();
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		nFrames = mTimeStepper.stepNumber(mClock.now() - now);
	}
	mNBackedUp = nFrames - 1;

	now = mClock.now();
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

bool FrameRateManager::isLeanFrame() const {
	return mNBackedUp >= mMaxBackedUp;
}

double FrameRateManager::getRealFPS() {
	if (mLastFrameTimes.size() == 0) {
		return 0.0;
	}
	double total = 0.0;
	for (uint i = 0; i < mLastFrameTimes.size(); i++) {
		total += mLastFrameTimes[i];
	}
	return mLastFrameTimes.size() / total;
}

double FrameRateManager::getLongestFrameTime() {
	if (mLastFrameTimes.size() == 0) {
		return 0.0;
	}
	return GEUtil::max(mLastFrameTimes);
}

double FrameRateManager::getLastFrameTime() {
	if (mLastFrameTimes.size() == 0) {
		return 0.0;
	}
	return mLastFrameTimes[mLastFrameTimes.size() - 1];
}
