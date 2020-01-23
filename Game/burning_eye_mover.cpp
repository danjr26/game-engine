#include "burning_eye_mover.h"
#include "game_engine.h"
#include "game.h"
#include "burning_eye.h"

burning_eye::Mover::Mover(BurningEye& i_parent) :
	mParent(i_parent) 
{}

burning_eye::Mover::~Mover()
{}

void burning_eye::Mover::update(double i_dt) {
	RigidBody2& rigidBody = mParent.getRigidBody();

	Vector2d linearVelocity = rigidBody.getLinearVelocity();
	URotation2d angularVelocity = rigidBody.getAngularVelocity();

	Controller::MoveCommand command = mParent.getController().getMoveCommand();
	Vector2d targetLinVel = command.mLinearVelocity;
	targetLinVel = targetLinVel.normalized() * geutil::min(targetLinVel.magnitude(), command.mMaxSpeed);
	Vector2d courseError = linearVelocity - targetLinVel;
	linearVelocity -= courseError.normalized() * geutil::min(courseError.magnitude(), command.mMaxAcceleration * i_dt);

	//mParent.getTransform().translateWorld(linearVelocity);

	mParent.getRigidBody().setLinearVelocity(linearVelocity);
	mParent.getRigidBody().setAngularVelocity(angularVelocity);
}


