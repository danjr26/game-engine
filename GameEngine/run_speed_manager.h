#ifndef RUN_SPEED_MANAGER_H
#define RUN_SPEED_MANAGER_H

#include "misc.h"

class RunSpeedManager {
public:
	enum Flags : uint64_t {

	};

private:
	double mMultipliers[64];

public:
	RunSpeedManager();
	double evaluate(double in_dt, uint64_t in_flags);
	double getMultiplier(uint8_t in_index);
	void setMultiplier(uint8_t in_index, double in_value);
};

#endif