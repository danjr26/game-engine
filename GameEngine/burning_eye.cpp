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

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 0.5));
	mRigidBody.setCollisionMask(mask);
	mRigidBody.setAngularMass(0);
	mRigidBody.setLinearMass(1.0);
	GE.physics().add(&mRigidBody);

	//mRigidBody.getTransform().setParent(&getTransform());
	getCollisionMask().addFilter(Game::CollisionFilters::enemy_ship);
	getCollisionMask().addFilter(Game::CollisionFilters::solid);
	GE.game().getMainCollisionContext().add(&getCollisionMask());

	GE.game().getDamageManager().add(&getCollisionMask(), &mHealth);

	initMembers();
}

burning_eye::BurningEye::~BurningEye() {
	if (GameEngine::exists()) {
		GE.perFrameUpdate().remove(this);
		GE.game().getMainCollisionContext().remove(&getCollisionMask());
		GE.game().getDamageManager().remove(&getCollisionMask());
		GE.physics().remove(&mRigidBody);
	}
}

void burning_eye::BurningEye::update(double in_dt) {
	if (mHealth.isDead()) {
		BurningEye* testEnemy = new BurningEye();
		testEnemy->getTransform().setLocalPosition(Vector2d(
			GEUtil::random<double>(-4, 4),
			GEUtil::random<double>(-4, 4)
			));
		testEnemy->getDepthTransform().setLocalDepth(-0.1);

		createBurningEyeExplosion(Vector3d(getTransform().getWorldPosition(), 0.0));
		GE.destruction().add(this);
	}

	mController.update(in_dt);
	mMover.update(in_dt);
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

CollisionMask2d& burning_eye::BurningEye::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}

CollisionQueue2d& burning_eye::BurningEye::getCollisionQueue() {
	return mCollisionQueue;
}

void burning_eye::BurningEye::initMembers() {
	mRenderer.init();
}
