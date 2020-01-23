#include "burning_eye.h"
#include "game_engine.h"
#include "game.h"
#include "particle_system2_specifiers.h"
#include "particle_system2_factories.h"

burning_eye::BurningEye::BurningEye() :
	mRenderer(*this),
	mMover(*this),
	mHealth(*this),
	mController(*this),
	mTarget(*this) {
	
	GE.perFrameUpdate().add(this);

	subTransform(&mRigidBody.getTransform());

	SphereCollider2d mask(Circled::fromPointRadius(Vector2d(), 0.5));
	mRigidBody.setCollider(mask);
	mRigidBody.setAngularMass(0);
	mRigidBody.setLinearMass(1.0);
	GE.physics().add(&mRigidBody);

	//mRigidBody.getTransform().setParent(&getTransform());
	getCollider().addFilter(Game::CollisionFilters::enemy_ship);
	getCollider().addFilter(Game::CollisionFilters::solid);
	GE.game().getMainCollisionContext().add(&getCollider());

	GE.game().getDamageManager().add(&getCollider(), &mHealth);

	initMembers();
}

burning_eye::BurningEye::~BurningEye() {
	if (GameEngine::exists()) {
		GE.perFrameUpdate().remove(this);
		GE.game().getMainCollisionContext().remove(&getCollider());
		GE.game().getDamageManager().remove(&getCollider());
		GE.physics().remove(&mRigidBody);
	}
}

void burning_eye::BurningEye::update(double i_dt) {
	if (mHealth.isDead()) {
		BurningEye* testEnemy = new BurningEye();
		testEnemy->getTransform().setLocalPosition(Vector2d(
			geutil::random<double>(-4, 4),
			geutil::random<double>(-4, 4)
			));
		testEnemy->getDepthTransform().setLocalDepth(-0.1);

		createBurningEyeExplosion(Vector3d(getTransform().getWorldPosition(), 0.0));
		GE.destruction().add(this);
	}

	mController.update(i_dt);
	mMover.update(i_dt);
}

void burning_eye::BurningEye::setMultiplier(double i_multiplier) {
	mMultiplier = i_multiplier;
	mRigidBody.setMultiplier(i_multiplier);
}

burning_eye::Renderer& burning_eye::BurningEye::getRenderer() {
	return mRenderer;
}

burning_eye::Mover& burning_eye::BurningEye::getMover() {
	return mMover;
}

burning_eye::Health& burning_eye::BurningEye::getHealth() {
	return mHealth;
}

burning_eye::Controller& burning_eye::BurningEye::getController() {
	return mController;
}

burning_eye::Target& burning_eye::BurningEye::getTarget() {
	return mTarget;
}

RigidBody<2>& burning_eye::BurningEye::getRigidBody() {
	return mRigidBody;
}

Collider2d& burning_eye::BurningEye::getCollider() {
	Collider2d* mask = mRigidBody.getCollider();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}

CollisionQueue2d& burning_eye::BurningEye::getCollisionQueue() {
	return mCollisionQueue;
}

void burning_eye::BurningEye::initMembers() {
	mRenderer.init();
}
