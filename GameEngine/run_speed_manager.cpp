#include "run_speed_manager.h"

RunSpeedManager::RunSpeedManager() {
	for (uint i = 0; i < 64; i++) {
		mMultipliers[i] = 0.0;
	}
}

double RunSpeedManager::evaluate(double in_dt, uint64_t in_flags) {
	for (uint i = 0; i < 64; i++) {
		if ((1ull << i) & in_flags) {
			in_dt *= mMultipliers[i];
		}
	}
	return in_dt;
}

double RunSpeedManager::getMultiplier(uint8_t in_index) {
	return mMultipliers[in_index];
}

void RunSpeedManager::setMultiplier(uint8_t in_index, double in_value) {
	mMultipliers[in_index] = in_value;
}
