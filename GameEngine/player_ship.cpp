#include "player_ship.h"
#include "game_engine.h"
#include "basic_collision_mask.h"
#include "game.h"

player_ship::PlayerShip::PlayerShip() :
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

player_ship::PlayerShip::~PlayerShip() {
	if (GameEngine::exists()) {
		GE.perFrameUpdate().remove(this);
		GE.game().getMainCollisionContext().remove(&getCollisionMask());
		GE.game().getDamageManager().remove(&getCollisionMask());
		GE.physics().remove(&mRigidBody);
	}
}

player_ship::Renderer& player_ship::PlayerShip::getRenderer() {
	return mRenderer;
}

player_ship::Mover& player_ship::PlayerShip::getMover() {
	return mMover;
}

player_ship::CameraMover& player_ship::PlayerShip::getCameraMover() {
	return mCameraMover;
}

RigidBody<2>& player_ship::PlayerShip::getRigidBody() {
	return mRigidBody;
}

CollisionMask2d& player_ship::PlayerShip::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}

player_ship::WeaponsSystem& player_ship::PlayerShip::getWeaponsSystem() {
	return mWeaponsSystem;
}

player_ship::Target& player_ship::PlayerShip::getTarget() {
	return mTarget;
}

player_ship::Health& player_ship::PlayerShip::getHealth() {
	return mHealth;
}

CollisionQueue2d& player_ship::PlayerShip::getCollisionQueue() {
	return mCollisionQueue;
}

void player_ship::PlayerShip::update(double in_dt) {
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

void player_ship::PlayerShip::setMultiplier(double in_multiplier) {
	mMultiplier = in_multiplier;
	mRigidBody.setMultiplier(in_multiplier);
}

void player_ship::PlayerShip::initMembers() {
	mRenderer.init();
}


