#include "test_enemy.h"
#include "game_engine.h"
#include "game.h"
#include "particle_system2_specifiers.h"
#include "particle_system2_factories.h"

TestEnemy::TestEnemy() :
	mRenderer(*this),
	mMover(*this),
	mHealth(*this) {
	
	GE.perFrameUpdate().add(this);

	subTransform(&mRigidBody.getTransform());

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 0.5));
	mRigidBody.setCollisionMask(mask);

	//mRigidBody.getTransform().setParent(&getTransform());
	getCollisionMask().addFilter(Game::MainCollisionContextFilters::enemy_ship);
	GE.game().getMainCollisionContext().add(&getCollisionMask());

	initMembers();
}

TestEnemy::~TestEnemy() {
	GE.perFrameUpdate().remove(this);
	GE.game().getMainCollisionContext().remove(&getCollisionMask());
}

void TestEnemy::update(double in_dt) {
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	CollisionContext2d& collisionContext = GE.game().getMainCollisionContext();
	collisionContext.getPartners(&getCollisionMask(), partners);

	CollisionPartner2d thisAsPartner;
	thisAsPartner.mMask = &getCollisionMask();

	for (auto it = partners.begin(); it < partners.end(); it++) {
		CollisionResponder* responder = (CollisionResponder*)(*it)->mMask->getParent();
		thisAsPartner.mCollision = (*it)->mCollision;
		if (responder) {
			CollisionPacket collisionPacket;
			(*responder)(thisAsPartner, collisionPacket);
			mHealth.damage(collisionPacket.mDamage);
		}
	}

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

RigidBody<2>& TestEnemy::getRigidBody() {
	return mRigidBody;
}

CollisionMask2d& TestEnemy::getCollisionMask() {
	CollisionMask2d* mask = mRigidBody.getCollisionMask();
	if (mask == nullptr) throw InvalidArgumentException();
	return *mask;
}

void TestEnemy::initMembers() {
	mRenderer.init();
}
