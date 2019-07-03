#include "player_ship_target.h"
#include "game_engine.h"
#include "game.h"

player_ship::Target::Target(PlayerShip& in_parent) :
	mParent(in_parent) {
	
	GE.game().getAITargets().insert(this);
}

player_ship::Target::~Target() {
	if (GameEngine::exists()) {
		GE.game().getAITargets().erase(this);
	}
}

CollisionMask2d const& player_ship::Target::getCollisionMask() const {
	return mParent.getCollisionMask();
}

Vector2d player_ship::Target::getLinearVelocity() const {
	return mParent.getRigidBody().getLinearVelocity();
}

URotation2d player_ship::Target::getAngularVelocity() const {
	return mParent.getRigidBody().getLinearVelocity();
}

Allegiance player_ship::Target::getAllegiance() const {
	return Allegiance::player;
}
