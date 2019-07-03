#include "burning_eye_target.h"
#include "burning_eye.h"
#include "game_engine.h"
#include "game.h"

burning_eye::Target::Target(BurningEye& in_parent) :
	mParent(in_parent) {
	
	GE.game().getAITargets().insert(this);
}

burning_eye::Target::~Target() {
	if (GameEngine::exists()) {
		GE.game().getAITargets().erase(this);
	}
}

CollisionMask2d const& burning_eye::Target::getCollisionMask() const {
	return mParent.getCollisionMask();
}

Vector2d burning_eye::Target::getLinearVelocity() const {
	return mParent.getRigidBody().getLinearVelocity();
}

URotation2d burning_eye::Target::getAngularVelocity() const {
	return mParent.getRigidBody().getAngularVelocity();
}

Allegiance burning_eye::Target::getAllegiance() const {
	return Allegiance::enemy;
}


