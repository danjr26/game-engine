#include "burning_eye_mover.h"
#include "game_engine.h"
#include "game.h"
#include "burning_eye.h"

burning_eye::Mover::Mover(BurningEye& in_parent) :
	mParent(in_parent) 
{}

burning_eye::Mover::~Mover()
{}

void burning_eye::Mover::update(double in_dt) {
	RigidBody2& rigidBody = mParent.getRigidBody();

	Vector2d linearVelocity = rigidBody.getLinearVelocity();
	URotation2d angularVelocity = rigidBody.getAngularVelocity();

	Controller::MoveCommand command = mParent.getController().getMoveCommand();
	Vector2d targetLinVel = command.mLinearVelocity;
	targetLinVel = targetLinVel.normalized() * GEUtil::min(targetLinVel.magnitude(), command.mMaxSpeed);
	Vector2d courseError = linearVelocity - targetLinVel;
	linearVelocity -= courseError.normalized() * GEUtil::min(courseError.magnitude(), command.mMaxAcceleration * in_dt);

	//mParent.getTransform().translateWorld(linearVelocity);

	mParent.getRigidBody().setLinearVelocity(linearVelocity);
	mParent.getRigidBody().setAngularVelocity(angularVelocity);
}


