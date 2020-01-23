#include "player_ship_target.h"
#include "game_engine.h"
#include "game.h"

player_ship::Target::Target(PlayerShip& i_parent) :
	mParent(i_parent) {
	
	GE.game().getAITargets().insert(this);
}

player_ship::Target::~Target() {
	if (GameEngine::exists()) {
		GE.game().getAITargets().erase(this);
	}
}

Collider2d const& player_ship::Target::getCollider() const {
	return mParent.getCollider();
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
