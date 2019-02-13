#ifndef TEST_ENEMY_MOVER_H
#define TEST_ENEMY_MOVER_H

#include "per_frame_updateable_object.h"

class TestEnemy;

class TestEnemyMover : public PerFrameUpdateableObject {
private:
	TestEnemy& mParent;

public:
	TestEnemyMover(TestEnemy& in_parent);

	void update(double in_dt) override;
};

#endif