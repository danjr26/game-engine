#include "player_ship_target.h"
#include "game_engine.h"
#include "game.h"

PlayerShipTarget::PlayerShipTarget(PlayerShip& in_parent) :
	mParent(in_parent) {
	
	GE.game().getAITargets().insert(this);
}

PlayerShipTarget::~PlayerShipTarget() {
	if (GameEngine::exists()) {
		GE.game().getAITargets().erase(this);
	}
}

CollisionMask2d const& PlayerShipTarget::getCollisionMask() const {
	return mParent.getCollisionMask();
}

Vector2d PlayerShipTarget::getLinearVelocity() const {
	return mParent.getRigidBody().getLinearVelocity();
}

URotation2d PlayerShipTarget::getAngularVelocity() const {
	return mParent.getRigidBody().getLinearVelocity();
}

Allegiance PlayerShipTarget::getAllegiance() const {
	return Allegiance::player;
}
