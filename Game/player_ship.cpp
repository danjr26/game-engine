#include "player_ship.h"
#include "game_engine.h"
#include "basic_collider.h"
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

	CircleCollider<double> mask(Circled::fromPointRadius(Vector2d(), 0.25));
	mRigidBody.setCollider(mask);
	mRigidBody.setAngularMass(0);
	mRigidBody.setLinearMass(1.0);
	GE.physics().add(&mRigidBody);

	getCollider().addFilter(Game::CollisionFilters::player_ship);
	getCollider().setQueue(&mCollisionQueue);
	GE.game().getMainCollisionContext().add(&getCollider());

	GE.game().getDamageManager().add(&getCollider(), &mHealth);

	initMembers();
}

player_ship::PlayerShip::~PlayerShip() {
	if (GameEngine::exists()) {
		GE.perFrameUpdate().remove(this);
		GE.game().getMainCollisionContext().remove(&getCollider());
		GE.game().getDamageManager().remove(&getCollider());
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

Collider2d& player_ship::PlayerShip::getCollider() {
	Collider2d* mask = mRigidBody.getCollider();
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

void player_ship::PlayerShip::update(double i_dt) {
	for (auto it = mCollisionQueue.begin(); !mCollisionQueue.empty(); it = mCollisionQueue.begin()) {
		if (it->mMask->hasFilter(Game::CollisionFilters::solid)) {
			DamageReceiver* receiver = GE.game().getDamageManager().get(it->mMask);
			if (receiver) {
				DamagePacket packet;
				packet.mAmount = 10.0;
				packet.mMethod = DamagePacket::Method::bulk;
				packet.mImpulse = { it->mCollision.mPoint, getTransform().applyToLocalDirection(Vector2d(1, 0)) };
				receiver->receiveDamage(packet, i_dt);
			}
		}
		mCollisionQueue.pop_front();
	}

	mMover.update(i_dt);
	mCameraMover.update(i_dt);
	mWeaponsSystem.update(i_dt);
}

void player_ship::PlayerShip::setMultiplier(double i_multiplier) {
	mMultiplier = i_multiplier;
	mRigidBody.setMultiplier(i_multiplier);
}

void player_ship::PlayerShip::initMembers() {
	mRenderer.init();
}


