#ifndef TEST_WEAPON_H
#define TEST_WEAPON_H

#include "weapon.h"

class TestWeapon : public Weapon {
private:
	bool mIsFiring;
	double mAccum;
	double mReloadTime;
	double mSpreadAngle;
	double mRecoil;

public:
	TestWeapon();

	void update(double i_dt, Feedback* o_feedback);
	void startFire() override;
	void endFire() override;
};

#endif