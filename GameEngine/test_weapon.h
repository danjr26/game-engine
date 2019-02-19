#ifndef TEST_WEAPON_H
#define TEST_WEAPON_H

#include "weapon.h"

class TestWeapon : public Weapon {
private:
	bool mIsFiring;
	double mAccum;
	double mReloadTime;

public:
	TestWeapon();

	void update(double in_dt, Feedback* out_feedback);
	void startFire() override;
	void endFire() override;
};

#endif