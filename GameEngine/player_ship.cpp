#include "player_ship.h"
#include "game_engine.h"
#include "basic_collision_mask.h"
#include "game.h"

PlayerShip::PlayerShip() :
	mRenderer(*this),
	mMover(*this),
	mCameraMover(*this),
	mWeaponsSystem(*this),
	mTarget(*this),
	mHealth(*this),
	mRigidBody() {

	subTransform(&mRigidBody.getTransform());

	GE.perFrameUpdate().add(this);

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 0.25));
	mRigidBody.setCollisionMask(mask);
	mRigidBody.setAngularMass(0);
	mRigidBody.setLinearMass(1.0);
	GE.physics().add(&mRigidBody);

	getCollisionMask().addFilter(Game::CollisionFilters::player_ship);
	getCollisionMask().setQueue(&mCollisionQueue);
	GE.game().getMainCollisionContext().add(&getCollisionMask());

	GE.game().getDamageManager().add(&getCollisionMask(), &mHealth);

	initMembers();
}

PlayerShip::~PlayerShip() {
	if (GameEngine::exists()) {
		GE.perFrameUpdate().remove(this);
		GE.game().getMainCollisionContext().remove(&getCollisionMask());
		GE.game().getDamageManager().remove(&getCollisionMask());
		GE.physics().remove(&mRigidBody);
	}
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

PlayerShipTarget& PlayerShip::getTarget() {
	return mTarget;
}

PlayerShipHealth& PlayerShip::getHealth() {
	return mHealth;
}

CollisionQueue2d& PlayerShip::getCollisionQueue() {
	return mCollisionQueue;
}

void PlayerShip::update(double in_dt) {
	for (auto it = mCollisionQueue.begin(); !mCollisionQueue.empty(); it = mCollisionQueue.begin()) {
		if (it->mMask->hasFilter(Game::CollisionFilters::solid)) {
			DamageReceiver* receiver = GE.game().getDamageManager().get(it->mMask);
			if (receiver) {
				DamagePacket packet;
				packet.mAmount = 10.0;
				packet.mMethod = DamagePacket::Method::bulk;
				packet.mImpulse = { it->mCollision.mPoint, getTransform().applyToLocalDirection(Vector2d(1, 0)) };
				receiver->receiveDamage(packet, in_dt);
			}
		}
		mCollisionQueue.pop_front();
	}

	mMover.update(in_dt);
	mCameraMover.update(in_dt);
	mWeaponsSystem.update(in_dt);
}

void PlayerShip::initMembers() {
	mRenderer.init();
}


