#ifndef TEST_ENEMY_MOVER_H
#define TEST_ENEMY_MOVER_H

#include "per_frame_updateable_object.h"
#include "hermite_transitioner.h"
#include "stepper.h"

class TestEnemy;

class TestEnemyMover {
private:
	TestEnemy& mParent;
	Stepper mPathUpdateStepper;
	HermiteTransitioner<double, 2> mPath;
	double mTargetSpeed;
	double mMaxAcceleration;

public:
	TestEnemyMover(TestEnemy& in_parent);
	~TestEnemyMover();

	void update(double in_dt);

private:
	void updatePath();
};

#endif