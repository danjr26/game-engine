#include "player_ship.h"
#include "game_engine.h"
#include "basic_collision_mask.h"

PlayerShip::PlayerShip() :
	mRenderer(*this),
	mMover(*this),
	mCameraMover(*this),
	mRigidBody() {

	GE.perFrameUpdate().add(&mMover);
	GE.perFrameUpdate().add(&mCameraMover);

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 0.25));
	mRigidBody.setCollisionMask(mask);

	mRigidBody.getTransform().setParent(&mTransform);
	getCollisionMask().addFilter(Game::MainCollisionContextFilters::player_ship);
	GE.game().getMainCollisionContext().add(&getCollisionMask());
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


