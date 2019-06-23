#include "player_ship_mover.h"
#include "player_ship.h"
#include "game_engine.h"
#include "game.h"
#include "misc.h"

PlayerShipMover::PlayerShipMover(PlayerShip& in_parent) :
	mParent(in_parent),
	mPointerInput(&GE.game().getPointerInput()),
	mShipInput(&GE.game().getPlayerShipInput())
{}

void PlayerShipMover::update(double in_dt) {
	RigidBody2& rigidBody = mParent.getRigidBody();

	Vector2d linearVelocity = rigidBody.getLinearVelocity();
	URotation2d angularVelocity = rigidBody.getAngularVelocity();

	Vector2d direction(
		mShipInput.getContext()->getRange(PlayerShipInputContext::Ranges::move_x),
		mShipInput.getContext()->getRange(PlayerShipInputContext::Ranges::move_y) * -1
	);

	linearVelocity.addToMagnitude(-15.0 * in_dt);
	linearVelocity += direction * 30.0 * in_dt;
	linearVelocity = linearVelocity.normalized() * GEUtil::min(linearVelocity.magnitude(), 8.0);

	//mParent.getTransform().translateWorld(linearVelocity * in_dt);

	mShipInput.clearEvents();
	mPointerInput.clearEvents();

	Vector2d mousePosition(
		mPointerInput.getContext()->getRange(PointerInputContext::Range::main_x),
		mPointerInput.getContext()->getRange(PointerInputContext::Range::main_y)
	);

	mousePosition += Vector2d(GE.cameras().get(CameraManager::ID::main)->getTransform().getLocalPosition());

	Rotation2d targetRotation(mousePosition - mParent.getTransform().getLocalPosition());
	URotation2d rotationDiff = Rotation2d(mParent.getTransform().getLocalRotation(), targetRotation);

	angularVelocity = URotation2d(
		-GEUtil::sign(rotationDiff.getAngle()) * 3.0 *
		GEUtil::min(exp(abs(rotationDiff.getAngle())) - 1.0, 4 * PI)
	);

	mParent.getTransform().rotateLocal(angularVelocity * in_dt);

	rigidBody.setLinearVelocity(linearVelocity);
	rigidBody.setAngularVelocity(angularVelocity);
}

void PlayerShipMover::init() {}
