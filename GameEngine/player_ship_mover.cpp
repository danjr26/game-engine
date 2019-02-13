#include "player_ship_mover.h"
#include "player_ship.h"
#include "game_engine.h"

PlayerShipMover::PlayerShipMover(PlayerShip& in_parent) :
	mParent(in_parent),
	mVelocity(),
	mPointerInput(&GE.game().getPointerInput()),
	mShipInput(&GE.game().getPlayerShipInput())
{}

void PlayerShipMover::update(double in_dt) {
	Vector2d direction(
		mShipInput.getContext()->getRange(PlayerShipInputContext::Ranges::move_x),
		mShipInput.getContext()->getRange(PlayerShipInputContext::Ranges::move_y) * -1
	);

	mVelocity.addToMagnitude(-15.0 * in_dt);
	mVelocity += direction * 30.0 * in_dt;
	mVelocity = mVelocity.normalized() * GEUtil::min(mVelocity.magnitude(), 8.0);

	mParent.getTransform().translateWorld(mVelocity * in_dt);

	InputEvent _event;
	mShipInput.clearEvents();
	mPointerInput.clearEvents();

	Vector2d mousePosition(
		mPointerInput.getContext()->getRange(PointerInputContext::Range::main_x),
		mPointerInput.getContext()->getRange(PointerInputContext::Range::main_y)
	);

	mousePosition += Vector2d(GE.cameras().get(CameraManager::ID::main)->getTransform().getLocalPosition());

	mParent.getTransform().setLocalRotation(Rotation2d(mousePosition - mParent.getTransform().getLocalPosition()));
	mParent.getTransform().rotateLocal(Rotation2d(PI / 2));
}
