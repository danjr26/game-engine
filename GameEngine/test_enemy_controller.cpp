#include "test_enemy_controller.h"
#include "game_engine.h"
#include "game.h"
#include "test_enemy.h"

TestEnemyController::TestEnemyController(TestEnemy& in_parent) :
	mParent(in_parent),
	mTarget(nullptr),
	mState(State::approach),
	mPathUpdateStepper(0.7),
	mPath(),
	mTargetSpeed(0.1),
	mMaxAcceleration(0.1) {

	updateTarget();
	updatePath();
}

TestEnemyController::~TestEnemyController() {}

void TestEnemyController::update(double in_dt) {
	if (mPathUpdateStepper.stepNumber(in_dt)) updatePath();
}

TestEnemyController::MoveCommand TestEnemyController::getMoveCommand() {
	MoveCommand command;
	command.mLinearVelocity = mPath.evaluate(mPathUpdateStepper.mAccum).mVector;
	command.mTargetSpeed = mTargetSpeed;
	command.mMaxAcceleration = mMaxAcceleration;
	return command;
}

void TestEnemyController::updatePath() {
	RigidBody2& rigidBody = mParent.getRigidBody();
	Vector2d linearVelocity = rigidBody.getLinearVelocity();
	URotation2d angularVelocity = rigidBody.getAngularVelocity();

	switch (mState) {
	case State::approach:
		break;
	case State::retreat: 
		break;
	case State::rush:
		break;
	default:
		break;
	}

	PlayerShip* target = GE.game().getPlayerShip();

	Vector2d currentPoint = mParent.getTransform().getWorldPosition();
	Vector2d targetPoint = target->getTransform().getWorldPosition() +
		target->getRigidBody().getLinearVelocity() * 1.0 +
		Vector2d(10, 0).rotated(GEUtil::random<double>(0, 2 * PI));
	Vector2d offset = targetPoint - currentPoint;

	LocatedVector2d currentLocVec(currentPoint, linearVelocity);
	LocatedVector2d targetLocVec(targetPoint, offset.normalized() * mTargetSpeed);

	mPath.mKeys.clear();
	mPath.mKeys.push_back({ currentLocVec, offset.magnitude() / mTargetSpeed });
	mPath.mKeys.push_back({ targetLocVec, 0.0 });
}

void TestEnemyController::updateTarget() {
	AITarget* bestTarget = nullptr;
	double bestDistance = std::numeric_limits<double>::infinity();
	Game::AITargetContainer& mTargets = GE.game().getAITargets();
	for (auto it = mTargets.begin(); it != mTargets.end(); it++) {
		if ((*it)->getAllegiance() == Allegiance::player) {
			double distance = ((*it)->getCollisionMask().getTransform().getWorldPosition() - mParent.getTransform().getWorldPosition()).magnitude();
			if (distance < bestDistance) {
				bestDistance = distance;
				bestTarget = *it;
			}
		}
	}

	mTarget = bestTarget;
}

