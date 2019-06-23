#include "test_enemy.h"
#include "game_engine.h"
#include "game.h"
#include "particle_system2_specifiers.h"
#include "particle_system2_factories.h"

TestEnemy::TestEnemy() :
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

TestEnemy::~TestEnemy() {
	if (GameEngine::exists()) {
		GE.perFrameUpdate().remove(this);
		GE.game().getMainCollisionContext().remove(&getCollisionMask());
		GE.game().getDamageManager().remove(&getCollisionMask());
		GE.physics().remove(&mRigidBody);
	}
}

void TestEnemy::update(double in_dt) {
	if (mHealth.isDead()) {
		TestEnemy* testEnemy = new TestEnemy();
		testEnemy->getTransform().setLocalPosition(Vector2d(
			GEUtil::random<double>(-4, 4),
			GEUtil::random<double>(-4, 4)
			));
		testEnemy->getDepthTransform().setLocalDepth(-0.1);

		createTestEnemyExplosion(Vector3d(getTransform().getLocalPosition(), 0.0));
		GE.destruction().add(this);
	}

	mController.update(in_dt);
	mMover.update(in_dt);
}

TestEnemyRenderer& TestEnemy::getRenderer() {
	return mRenderer;
}

TestEnemyMover& TestEnemy::getMover() {
	return mMover;
}

TestEnemyHealth& TestEnemy::getHealth() {
	return mHealth;
}

TestEnemyController& TestEnemy::getController() {
	return mController;
}

TestEnemyTarget& TestEnemy::getTarget() {
	return mTarget;
}

RigidBody<2>& TestEnemy::getRigidBody() {
	return mRigidBody;
}

CollisionMask2d& TestEnemy::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}

CollisionQueue2d& TestEnemy::getCollisionQueue() {
	return mCollisionQueue;
}

void TestEnemy::initMembers() {
	mRenderer.init();
}
