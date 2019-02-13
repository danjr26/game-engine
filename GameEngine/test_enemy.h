#ifndef TEST_ENEMY_H
#define TEST_ENEMY_H

#include "test_enemy_renderer.h"
#include "test_enemy_mover.h"
#include "deep_transformable_object.h"

class TestEnemy : public DeepTransformableObject2d {
private:
	TestEnemyRenderer mRenderer;
	TestEnemyMover mMover;

public:
	TestEnemy();
};

#endif