#ifndef TEST_ENEMY_H
#define TEST_ENEMY_H

#include "test_enemy_renderer.h"
#include "test_enemy_mover.h"
#include "deep_transformable_object.h"
#include "rigid_body.h"
#include "destructable_object.h"

class TestEnemy : public DeepTransformableObject2d, public DestructableObject {
private:
	TestEnemyRenderer mRenderer;
	TestEnemyMover mMover;
	RigidBody<2> mRigidBody;

public:
	TestEnemy();

	TestEnemyRenderer& getRenderer();
	TestEnemyMover& getMover();
	RigidBody<2>& getRigidBody();
	CollisionMask2d& getCollisionMask();

private:
	void initMembers();
};

#endif