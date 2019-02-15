#include "test_enemy.h"
#include "game_engine.h"

TestEnemy::TestEnemy() :
	mRenderer(*this),
	mMover(*this) {
	
	GE.perFrameUpdate().add(&mMover);

	CircleCollisionMask<double> mask(Circled::fromPointRadius(Vector2d(), 0.5));
	mRigidBody.setCollisionMask(mask);

	mRigidBody.getTransform().setParent(&mTransform);
	getCollisionMask().addFilter(Game::MainCollisionContextFilters::enemy);
	GE.game().getMainCollisionContext().add(&getCollisionMask());
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
