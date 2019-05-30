#ifndef MINIGUN_H
#define MINIGUN_H

#include "weapon.h"

class Minigun : public Weapon {
private:
	enum class State {
		off,
		warm,
		on
	} mState;
	double mAccum;
	double mReloadTime;
	double mWarmTime;
	double mSpreadAngle;
	double mRecoil;

public:
	Minigun();

	void update(double in_dt, Feedback* out_feedback);
	void startFire() override;
	void endFire() override;
};

#endif