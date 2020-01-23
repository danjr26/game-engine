#include "player_ship_camera_mover.h"
#include "player_ship.h"
#include "game_engine.h"
#include "game.h"

player_ship::CameraMover::CameraMover(PlayerShip& i_parent) :
	mParent(i_parent),
	mPointerInput(&GE.game().getPointerInput())
{}

void player_ship::CameraMover::update(double i_dt) {
	Camera& camera = *GE.cameras().get(CameraManager::ID::main);

	Vector2d centerOffset = Vector2d((camera.getProjection().mMaxima - camera.getProjection().mMinima) / 2.0);
	centerOffset.y() = -centerOffset.y();

	Vector2d current = Vector2d(camera.getTransform().getLocalPosition());

	Vector2d mousePosition = Vector2d(
		mPointerInput.getContext()->getRange(PointerInputContext::Range::mai_x),
		mPointerInput.getContext()->getRange(PointerInputContext::Range::mai_y)
	) + current;

	Vector2d lead = mousePosition - mParent.getTransform().getWorldPosition();
	lead = lead.normalized() * sqrt(lead.magnitude());
	Vector2d target = mParent.getTransform().getWorldPosition() + lead - centerOffset;
	Vector2d step = (target - current);
	double distance = step.magnitude();
	if (distance > 0.01) {
		step = step.normalized() * geutil::min(0.5 * exp(1.5 * distance), 10.0) * i_dt;
	}

	camera.getTransform().setLocalPosition(
		Vector3d(
			current + step,
			camera.getTransform().getLocalPosition().z()
		)
	);
}
