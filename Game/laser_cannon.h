#ifndef LASER_CANNON_H
#define LASER_CANNON_H

#include "weapon.h"

class LaserCannon : public Weapon {
private:
	enum class State {
		off,
		on
	} mState;
	double mAccum;
	double mReloadTime;
	double mSpreadAngle;
	double mRecoil;

public:
	LaserCannon();

	void update(double i_dt, Feedback* o_feedback);
	void startFire() override;
	void endFire() override;
};

#endif