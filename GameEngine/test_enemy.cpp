#include "test_enemy.h"
#include "game_engine.h"
#include "game.h"

TestEnemy::TestEnemy() :
	mRenderer(*this),
	mMover(*this) {
	
	GE.perFrameUpdate().add(&mMover);

	subTransform(&mRigidBody.getTransform());

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 0.5));
	mRigidBody.setCollisionMask(mask);

	//mRigidBody.getTransform().setParent(&getTransform());
	getCollisionMask().addFilter(Game::MainCollisionContextFilters::enemy_ship);
	GE.game().getMainCollisionContext().add(&getCollisionMask());

	initMembers();
}

TestEnemyRenderer& TestEnemy::getRenderer() {
	return mRenderer;
}

TestEnemyMover& TestEnemy::getMover() {
	return mMover;
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
