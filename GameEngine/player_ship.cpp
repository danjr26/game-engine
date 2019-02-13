#include "player_ship.h"
#include "game_engine.h"
#include "basic_collision_mask.h"

PlayerShip::PlayerShip() :
	mRenderer(*this),
	mMover(*this),
	mRigidBody() {

	GE.perFrameUpdate().add(&mMover);

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 1.0));
	mask.getTransform().setParent(&mTransform);
	mRigidBody.setCollisionMask(mask);
}

PlayerShipRenderer& PlayerShip::getRenderer() {
	return mRenderer;
}

PlayerShipMover& PlayerShip::getMover() {
	return mMover;
}

RigidBody<2>& PlayerShip::getRigidBody() {
	return mRigidBody;
}

CollisionMask2d& PlayerShip::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}


