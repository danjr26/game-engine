#include "player_ship_camera_mover.h"
#include "player_ship.h"
#include "game_engine.h"
#include "game.h"

PlayerShipCameraMover::PlayerShipCameraMover(PlayerShip& in_parent) :
	mParent(in_parent),
	mPointerInput(&GE.game().getPointerInput())
{}

void PlayerShipCameraMover::update(double in_dt) {
	Camera& camera = *GE.cameras().get(CameraManager::ID::main);

	Vector2d centerOffset = Vector2d((camera.getProjection().mMaxima - camera.getProjection().mMinima) / 2.0);
	centerOffset.y() = -centerOffset.y();

	Vector2d current = Vector2d(camera.getTransform().getLocalPosition());

	Vector2d mousePosition = Vector2d(
		mPointerInput.getContext()->getRange(PointerInputContext::Range::main_x),
		mPointerInput.getContext()->getRange(PointerInputContext::Range::main_y)
	) + current;

	Vector2d lead = mousePosition - mParent.getTransform().getWorldPosition();
	lead = lead.normalized() * sqrt(lead.magnitude());
	Vector2d target = mParent.getTransform().getWorldPosition() + lead - centerOffset;
	Vector2d step = (target - current);
	double distance = step.magnitude();
	if (distance > 0.01) {
		step = step.normalized() * GEUtil::min(0.5 * exp(1.5 * distance), 10.0) * in_dt;
	}

	camera.getTransform().setLocalPosition(
		Vector3d(
			current + step,
			camera.getTransform().getLocalPosition().z()
		)
	);
}
