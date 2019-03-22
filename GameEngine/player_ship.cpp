#include "player_ship.h"
#include "game_engine.h"
#include "basic_collision_mask.h"
#include "game.h"

PlayerShip::PlayerShip() :
	mRenderer(*this),
	mMover(*this),
	mCameraMover(*this),
	mWeaponsSystem(*this),
	mCollisionResponder(*this),
	mRigidBody() {

	subTransform(&mRigidBody.getTransform());

	GE.perFrameUpdate().add(this);

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 0.25));
	mRigidBody.setCollisionMask(mask);

	mRigidBody.setAngularMass(0);
	mRigidBody.setLinearMass(1.0);

	getCollisionMask().addFilter(Game::MainCollisionContextFilters::player_ship);
	getCollisionMask().setParent((CollisionResponder*)&mCollisionResponder);
	GE.game().getMainCollisionContext().add(&getCollisionMask());

	initMembers();
}

PlayerShipRenderer& PlayerShip::getRenderer() {
	return mRenderer;
}

PlayerShipMover& PlayerShip::getMover() {
	return mMover;
}

PlayerShipCameraMover& PlayerShip::getCameraMover() {
	return mCameraMover;
}

RigidBody<2>& PlayerShip::getRigidBody() {
	return mRigidBody;
}

CollisionMask2d& PlayerShip::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}

PlayerShipWeaponsSystem& PlayerShip::getWeaponsSystem() {
	return mWeaponsSystem;
}

PlayerShipCollisionResponder& PlayerShip::getCollisionResponder() {
	return mCollisionResponder;
}

void PlayerShip::update(double in_dt) {
	mMover.update(in_dt);
	mCameraMover.update(in_dt);
	mWeaponsSystem.update(in_dt);
}

void PlayerShip::initMembers() {
	mRenderer.init();
}


