#ifndef CHAi_BOLT_H
#define CHAi_BOLT_H

#include "weapon.h"

class ChainBolt : public Weapon {
private:
	enum class State {
		off,
		on
	} mState;
	double mAccum;
	double mReloadTime;
	double mRecoil;

public:
	ChainBolt();

	void update(double i_dt, Feedback* o_feedback);
	void startFire() override;
	void endFire() override;
};

#endif