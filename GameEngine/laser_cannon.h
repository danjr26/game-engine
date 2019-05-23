#ifndef LASER_CANNON_H
#define LASER_CANNON_H

#include "weapon.h"

class LaserCannon : public Weapon {
private:
	bool mIsFiring;
	double mAccum;
	double mReloadTime;
	double mSpreadAngle;
	double mRecoil;

public:
	LaserCannon();

	void update(double in_dt, Feedback* out_feedback);
	void startFire() override;
	void endFire() override;
};

#endif