#include "player_ship_mover.h"
#include "player_ship.h"

PlayerShipMover::PlayerShipMover(PlayerShip& in_parent) :
	mParent(in_parent),
	mVelocity()
{}

void PlayerShipMover::update(double in_dt) {
	Vector2d direction(
		mParent.getInput().getRange(PlayerShipInputContext::Ranges::move_x),
		mParent.getInput().getRange(PlayerShipInputContext::Ranges::move_y) * -1
	);

	mVelocity.addToMagnitude(-1000.0 * in_dt);
	mVelocity += direction * 2000.0 * in_dt;
	mVelocity = mVelocity.normalized() * GEUtil::min(mVelocity.magnitude(), 400.0);

	mParent.getTransform().translateWorld(mVelocity * in_dt);
}
