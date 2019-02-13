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

	camera.getTransform().setLocalPosition(
		Vector3d(
			mParent.getTransform().getWorldPosition() - centerOffset, 
			camera.getTransform().getLocalPosition().z()
		)
	);
}
