#include "burning_eye_target.h"
#include "burning_eye.h"
#include "game_engine.h"
#include "game.h"

BurningEyeTarget::BurningEyeTarget(BurningEye& in_parent) :
	mParent(in_parent) {
	
	GE.game().getAITargets().insert(this);
}

BurningEyeTarget::~BurningEyeTarget() {
	if (GameEngine::exists()) {
		GE.game().getAITargets().erase(this);
	}
}

CollisionMask2d const& BurningEyeTarget::getCollisionMask() const {
	return mParent.getCollisionMask();
}

Vector2d BurningEyeTarget::getLinearVelocity() const {
	return mParent.getRigidBody().getLinearVelocity();
}

URotation2d BurningEyeTarget::getAngularVelocity() const {
	return mParent.getRigidBody().getAngularVelocity();
}

Allegiance BurningEyeTarget::getAllegiance() const {
	return Allegiance::enemy;
}


