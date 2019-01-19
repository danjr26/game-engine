#include "frame_rate_manager.h"
#include "misc.h"
#include "log.h"
#include <thread>

FrameRateManager::FrameRateManager(double in_fps) :
	timestepper(1.0 / in_fps),
	nTrackedFrames(60),
	nBackedUp(0) {

	lastFrameTimes.reserve(nTrackedFrames);

	lastFrame = clock.Now();
}

void FrameRateManager::Set_FPS(double in_fps) {
	timestepper.step = 1.0 / in_fps;
}

double FrameRateManager::Get_FPS() {
	return 1.0 / timestepper.step;
}

void FrameRateManager::Set_Dt(double in_dt) {
	timestepper.step = in_dt;
}

double FrameRateManager::Get_Dt() {
	return timestepper.step;
}

void FrameRateManager::Reset_Timer() {
	lastFrame = clock.Now();
}

void FrameRateManager::Yield_Until_Next_Frame() {
	double now = clock.Now();
	uint nFrames = timestepper.Step_Number(now - lastFrame) + nBackedUp;

	while (nFrames == 0) {
		now = clock.Now();
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		nFrames = timestepper.Step_Number(clock.Now() - now);
	}
	nBackedUp = nFrames - 1;

	now = clock.Now();
	double thisFrameTime = now - lastFrame;
	lastFrame = now;

	if (lastFrameTimes.size() < nTrackedFrames) {
		lastFrameTimes.push_back(thisFrameTime);
	}
	else {
		for (uint i = 0; i < lastFrameTimes.size() - 1; i++) {
			lastFrameTimes[i] = lastFrameTimes[i + 1];
		}
		lastFrameTimes[lastFrameTimes.size() - 1] = thisFrameTime;
	}
}

double FrameRateManager::Get_Real_FPS() {
	if (lastFrameTimes.size() == 0) {
		return 0.0;
	}
	double total = 0.0;
	for (uint i = 0; i < lastFrameTimes.size(); i++) {
		total += lastFrameTimes[i];
	}
	return lastFrameTimes.size() / total;
}

double FrameRateManager::Get_Longest_Frame_Time() {
	if (lastFrameTimes.size() == 0) {
		return 0.0;
	}
	return Max(lastFrameTimes);
}

double FrameRateManager::Get_Last_Frame_Time() {
	if (lastFrameTimes.size() == 0) {
		return 0.0;
	}
	return lastFrameTimes[lastFrameTimes.size() - 1];
}
