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
#include "damage_receiver.h"
#include "test_enemy_controller.h"
#include "test_enemy_target.h"

class TestEnemy : public DeepTransformableObject2d, public DestructableObject, 
	public PerFrameUpdateableObject {
private:
	TestEnemyRenderer mRenderer;
	TestEnemyMover mMover;
	TestEnemyHealth mHealth;
	TestEnemyController mController;
	TestEnemyTarget mTarget;
	RigidBody<2> mRigidBody;
	CollisionQueue2d mCollisionQueue;

public:
	TestEnemy();
	~TestEnemy();

	void update(double in_dt) override;

	TestEnemyRenderer& getRenderer();
	TestEnemyMover& getMover();
	TestEnemyHealth& getHealth();
	TestEnemyController& getController();
	TestEnemyTarget& getTarget();
	RigidBody<2>& getRigidBody();
	CollisionMask2d& getCollisionMask();
	CollisionQueue2d& getCollisionQueue();

private:
	void initMembers();
};

#endif