#ifndef TEST_ENEMY_H
#define TEST_ENEMY_H

#include "test_enemy_renderer.h"
#include "test_enemy_health.h"
#include "test_enemy_mover.h"
#include "deep_transformable_object.h"
#include "rigid_body.h"
#include "destructable_object.h"
#include "per_frame_updateable_object.h"
#include "collision_queue.h"

class TestEnemy : public DeepTransformableObject2d, public DestructableObject, public PerFrameUpdateableObject {
private:
	TestEnemyRenderer mRenderer;
	TestEnemyMover mMover;
	TestEnemyHealth mHealth;
	RigidBody<2> mRigidBody;
	CollisionQueue2d mCollisionQueue;

public:
	TestEnemy();
	~TestEnemy();

	void update(double in_dt) override;

	TestEnemyRenderer& getRenderer();
	TestEnemyMover& getMover();
	TestEnemyHealth& getHealth();
	RigidBody<2>& getRigidBody();
	CollisionMask2d& getCollisionMask();
	CollisionQueue2d& getCollisionQueue();

private:
	void initMembers();
};

#endif