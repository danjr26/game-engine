#ifndef FRAME_RATE_MANAGER_H
#define FRAME_RATE_MANAGER_H

#include "stepper.h"
#include "clock.h"
#include "definitions.h"
#include <vector>

class FrameRateManager {
private:
	Clock clock;
	Stepper timestepper;
	double lastFrame;

	uint nTrackedFrames;
	std::vector<double> lastFrameTimes;

public:
	FrameRateManager(double in_fps = 60.0);
	void Set(double in_fps);
	double Get();
	void Reset_Timer();
	void Yield_Until_Next_Frame();
	double Get_Real_FPS();
	double Get_Last_Frame_Time();
};

#endif