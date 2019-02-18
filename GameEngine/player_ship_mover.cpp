#include "player_ship_mover.h"
#include "player_ship.h"
#include "game_engine.h"
#include "misc.h"

PlayerShipMover::PlayerShipMover(PlayerShip& in_parent) :
	mParent(in_parent),
	mLinearVelocity(),
	mAngularVelocity(),
	mPointerInput(&GE.game().getPointerInput()),
	mShipInput(&GE.game().getPlayerShipInput())
{}

void PlayerShipMover::update(double in_dt) {
	Vector2d direction(
		mShipInput.getContext()->getRange(PlayerShipInputContext::Ranges::move_x),
		mShipInput.getContext()->getRange(PlayerShipInputContext::Ranges::move_y) * -1
	);

	mLinearVelocity.addToMagnitude(-15.0 * in_dt);
	mLinearVelocity += direction * 30.0 * in_dt;
	mLinearVelocity = mLinearVelocity.normalized() * GEUtil::min(mLinearVelocity.magnitude(), 8.0);

	mParent.getTransform().translateWorld(mLinearVelocity * in_dt);

	InputEvent _event;
	mShipInput.clearEvents();
	mPointerInput.clearEvents();

	Vector2d mousePosition(
		mPointerInput.getContext()->getRange(PointerInputContext::Range::main_x),
		mPointerInput.getContext()->getRange(PointerInputContext::Range::main_y)
	);

	mousePosition += Vector2d(GE.cameras().get(CameraManager::ID::main)->getTransform().getLocalPosition());

	Rotation2d targetRotation(mousePosition - mParent.getTransform().getLocalPosition());
	URotation2d rotationDiff = Rotation2d(mParent.getTransform().getLocalRotation(), targetRotation);

	mAngularVelocity = URotation2d(-GEUtil::sign(rotationDiff.getAngle()) * GEUtil::min(exp(abs(rotationDiff.getAngle())), 2 * PI));

	mParent.getTransform().rotateLocal(mAngularVelocity * in_dt);
}
