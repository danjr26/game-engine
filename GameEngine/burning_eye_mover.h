#ifndef TEST_ENEMY_MOVER_H
#define TEST_ENEMY_MOVER_H

#include "per_frame_updateable_object.h"
#include "hermite_transitioner.h"
#include "stepper.h"

class BurningEye;

class BurningEyeMover {
private:
	BurningEye& mParent;

public:
	BurningEyeMover(BurningEye& in_parent);
	~BurningEyeMover();

	void update(double in_dt);
};

#endif