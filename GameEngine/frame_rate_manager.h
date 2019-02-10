#ifndef FRAME_RATE_MANAGER_H
#define FRAME_RATE_MANAGER_H

#include "stepper.h"
#include "clock.h"
#include "definitions.h"
#include <vector>

class FrameRateManager {
private:
	Clock mClock;
	Stepper mTimeStepper;
	uint mNBackedUp;
	uint mMaxBackedUp;
	double mLastFrame;

	uint mNTrackedFrames;
	std::vector<double> mLastFrameTimes;

public:
	FrameRateManager(double in_fps = 60.0);
	void setFPS(double in_fps);
	double getFPS() const;
	void setDt(double in_dt);
	double getDt() const;
	void resetTimer();
	void yieldUntilNextFrame();
	bool isLeanFrame() const;
	double getRealFPS();
	double getLongestFrameTime();
	double getLastFrameTime();
};

#endif