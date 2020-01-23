#ifndef INDUSTRIAL_LASER_H
#define INDUSTRIAL_LASER_H

#include "weapon.h"
#include "industrial_laser_beam.h"

class IndustrialLaser : public Weapon {
private:
	IndustrialLaserBeam mBeam;
	enum class State {
		off,
		warm,
		on
	} mState;
	double mWarmTime;
	double mAccum;

public:
	IndustrialLaser();

	void update(double i_dt, Feedback* o_feedback);
	void startFire() override;
	void endFire() override;

private:
	void pushIgnoreMasks() override;
};

#endif