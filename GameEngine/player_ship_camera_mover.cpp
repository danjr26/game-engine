#include "player_ship_camera_mover.h"
#include "player_ship.h"
#include "game_engine.h"

PlayerShipCameraMover::PlayerShipCameraMover(PlayerShip& in_parent) :
	mParent(in_parent)
{}

void PlayerShipCameraMover::update(double in_dt) {
	Camera& camera = *GE.cameras().get(CameraManager::ID::main);

	Vector2d centerOffset = Vector2d((camera.getProjection().mMaxima - camera.getProjection().mMinima) / 2.0);
	centerOffset.y() = -centerOffset.y();

	Vector2d current = Vector2d(camera.getTransform().getLocalPosition());
	Vector2d target = mParent.getTransform().getWorldPosition() - centerOffset;
	Vector2d step = (target - current);
	double distance = step.magnitude();
	if (distance > 0.01) {
		step = step.normalized() * GEUtil::min(exp(distance), 10.0) * in_dt;
	}

	camera.getTransform().setLocalPosition(
		Vector3d(
			current + step,
			camera.getTransform().getLocalPosition().z()
		)
	);
}
