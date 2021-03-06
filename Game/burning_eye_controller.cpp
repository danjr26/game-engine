#include "burning_eye_controller.h"
#include "game_engine.h"
#include "game.h"
#include "burning_eye.h"

burning_eye::Controller::Controller(BurningEye& i_parent) :
	mParent(i_parent),
	mTarget(nullptr),
	mState(State::approach),
	mStateT(0.0),
	mPathUpdateStepper(0.7),
	mPath(),
	mMaxSpeed(5.0),
	mMaxAcceleration(5.0) {

	updateTarget();
	updatePath();
}

burning_eye::Controller::~Controller() {}

void burning_eye::Controller::update(double i_dt) {
	if (mTarget == nullptr) return;

	bool forcePathUpdate = false;

	Vector2d targetToParent = mParent.getTransform().getWorldPosition() - mTarget->getCollider().getTransform().getWorldPosition();

	bool beingRushed = targetToParent.theta(mTarget->getLinearVelocity()) < (PI / 12) && mTarget->getLinearVelocity().magnitude() > 4.0;

	switch (mState) {
	case State::approach:
		if (targetToParent.magnitude() <= 5.0) {
			mState = State::rush;
			forcePathUpdate = true;
			mStateT = 0.0;
		}
		else if (beingRushed) {
			mState = State::retreat;
			forcePathUpdate = true;
			mStateT = 0.0;
		}
		break;
	case State::retreat:
		if (targetToParent.magnitude() >= 10.0 && mStateT > 1.0) {
			mState = State::approach;
			forcePathUpdate = true;
			mStateT = 0.0;
		}
		break;
	case State::rush:
		if (beingRushed && targetToParent.magnitude() >= 2.0 || mStateT >= 2.0) {
			mState = State::retreat;
			forcePathUpdate = true;
			mStateT = 0.0;
		}
		break;
	}

	if (mPathUpdateStepper.stepNumber(i_dt) || forcePathUpdate) updatePath();
	mStateT += i_dt;
}

burning_eye::Controller::MoveCommand burning_eye::Controller::getMoveCommand() {
	MoveCommand command;
	command.mLinearVelocity = mPath.evaluate(mPathUpdateStepper.mAccum).mVector;
	command.mMaxSpeed = mMaxSpeed;
	command.mMaxAcceleration = mMaxAcceleration;
	return command;
}

void burning_eye::Controller::updatePath() {
	if (mTarget == nullptr) return;

	RigidBody2& rigidBody = mParent.getRigidBody();
	Vector2d linearVelocity = rigidBody.getLinearVelocity();
	URotation2d angularVelocity = rigidBody.getAngularVelocity();

	Vector2d entityPosition = mTarget->getCollider().getTransform().getWorldPosition();
	Vector2d entityLinearVelocity = mTarget->getLinearVelocity();

	Vector2d targetPoint;
	Vector2d currentPoint = mParent.getTransform().getWorldPosition();

	switch (mState) {
	case State::approach:
	{
		mMaxSpeed = 7.0;
		mMaxAcceleration = 10.0;
		mPathUpdateStepper.mStep = 0.7;
		targetPoint = entityPosition + entityLinearVelocity + Vector2d(10, 0).rotated(geutil::random<double>(0, 2 * PI));
		break;
	}
	case State::retreat:
	{
		mMaxSpeed = 8.0;
		mMaxAcceleration = 15.0;
		mPathUpdateStepper.mStep = 0.5;
		targetPoint = (currentPoint - entityPosition).rotated(geutil::random<double>(-PI / 4, PI / 4));
		targetPoint.addToMagnitude(1.0);
		break;
	}
	case State::rush:
	{
		mMaxSpeed = 12.0;
		mMaxAcceleration = 25.0;
		mPathUpdateStepper.mStep = 1.0;
		targetPoint = entityPosition + entityLinearVelocity * 0.25/* + Vector2d(2, 0).rotated(geutil::random<double>(0, 2 * PI))*/;
		break;
	}
	}

	Vector2d offset = targetPoint - currentPoint;

	LocatedVector2d currentLocVec(currentPoint, linearVelocity);
	LocatedVector2d targetLocVec(targetPoint, offset.normalized() * mMaxSpeed);

	mPath.mKeys.clear();
	mPath.mKeys.push_back({ currentLocVec, offset.magnitude() / mMaxSpeed / 2.0 });
	mPath.mKeys.push_back({ targetLocVec, 0.0 });
}

void burning_eye::Controller::updateTarget() {
	AITarget* bestTarget = nullptr;
	double bestDistance = std::numeric_limits<double>::infinity();
	Game::AITargetContainer& mTargets = GE.game().getAITargets();
	for (auto it = mTargets.begin(); it != mTargets.end(); it++) {
		if ((*it)->getAllegiance() == Allegiance::player) {
			double distance = ((*it)->getCollider().getTransform().getWorldPosition() - mParent.getTransform().getWorldPosition()).magnitude();
			if (distance < bestDistance) {
				bestDistance = distance;
				bestTarget = *it;
			}
		}
	}

	mTarget = bestTarget;
}

