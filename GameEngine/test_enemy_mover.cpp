#include "test_enemy_mover.h"
#include "game_engine.h"
#include "game.h"
#include "test_enemy.h"

TestEnemyMover::TestEnemyMover(TestEnemy& in_parent) :
	mParent(in_parent),
	mPathUpdateStepper(0.3),
	mPath(),
	mTargetSpeed(0.1),
	mMaxAcceleration(0.5) {

	updatePath();
}

TestEnemyMover::~TestEnemyMover() 
{}

void TestEnemyMover::update(double in_dt) {
	PlayerShip* target = GE.game().getPlayerShip();
	RigidBody2& rigidBody = mParent.getRigidBody();

	Vector2d linearVelocity = rigidBody.getLinearVelocity();
	URotation2d angularVelocity = rigidBody.getAngularVelocity();

	if (mPathUpdateStepper.stepNumber(in_dt)) updatePath();

	Vector2d targetLinVel = mPath.evaluate(mPathUpdateStepper.mAccum).mVector;
	targetLinVel = targetLinVel.normalized() * GEUtil::min(targetLinVel.magnitude(), mTargetSpeed);
	Vector2d courseError = linearVelocity - targetLinVel;
	linearVelocity -= courseError.normalized() * GEUtil::min(courseError.magnitude(), mMaxAcceleration * in_dt);

	mParent.getTransform().translateWorld(linearVelocity);

	mParent.getRigidBody().setLinearVelocity(linearVelocity);
	mParent.getRigidBody().setAngularVelocity(angularVelocity);
}

void TestEnemyMover::updatePath() {
	RigidBody2& rigidBody = mParent.getRigidBody();	
	Vector2d linearVelocity = rigidBody.getLinearVelocity();
	URotation2d angularVelocity = rigidBody.getAngularVelocity();

	PlayerShip* target = GE.game().getPlayerShip();

	Vector2d currentPoint = mParent.getTransform().getWorldPosition();
	Vector2d targetPoint = target->getTransform().getWorldPosition() + target->getRigidBody().getLinearVelocity() * 1.0;
	Vector2d offset = targetPoint - currentPoint;

	LocatedVector2d currentLocVec(currentPoint, linearVelocity);
	LocatedVector2d targetLocVec(targetPoint, offset.normalized() * mTargetSpeed);

	mPath.mKeys.clear();
	mPath.mKeys.push_back({ currentLocVec, offset.magnitude() / mTargetSpeed });
	mPath.mKeys.push_back({ targetLocVec, 0.0 });
}
