#ifndef TEST_ENEMY_CONTROLLER_H
#define TEST_ENEMY_CONTROLLER_H

#include "located_vector.h"
#include "hermite_transitioner.h"
#include "stepper.h"
#include "ai_target.h"

class BurningEye;

class BurningEyeController {
public:
	struct MoveCommand {
		Vector2d mLinearVelocity;
		double mMaxSpeed;
		double mMaxAcceleration;
	};

private:
	enum class State {
		approach,
		rush,
		retreat
	};

	BurningEye& mParent;
	AITarget* mTarget;
	State mState;
	double mStateT;
	Stepper mPathUpdateStepper;
	HermiteTransitioner<double, 2> mPath;
	double mMaxSpeed;
	double mMaxAcceleration;

public:
	BurningEyeController(BurningEye& in_parent);
	~BurningEyeController();

	void update(double in_dt);
	MoveCommand getMoveCommand();
	
private:
	void updatePath();
	void updateTarget();
};

#endif