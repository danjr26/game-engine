#include "test_enemy_target.h"
#include "test_enemy.h"
#include "game_engine.h"
#include "game.h"

TestEnemyTarget::TestEnemyTarget(TestEnemy& in_parent) :
	mParent(in_parent) {
	
	GE.game().getAITargets().insert(this);
}

TestEnemyTarget::~TestEnemyTarget() {
	if (GameEngine::exists()) {
		GE.game().getAITargets().erase(this);
	}
}

CollisionMask2d const& TestEnemyTarget::getCollisionMask() const {
	return mParent.getCollisionMask();
}

Vector2d TestEnemyTarget::getLinearVelocity() const {
	return mParent.getRigidBody().getLinearVelocity();
}

URotation2d TestEnemyTarget::getAngularVelocity() const {
	return mParent.getRigidBody().getAngularVelocity();
}

Allegiance TestEnemyTarget::getAllegiance() const {
	return Allegiance::enemy;
}


