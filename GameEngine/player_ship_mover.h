#ifndef PLAYER_SHIP_MOVER_H
#define PLAYER_SHIP_MOVER_H

#include "player_ship_input_context.h"
#include "pointer_input_context.h"
#include "per_frame_updateable_object.h"
#include "cubic_transitioner.h"
#include "vector.h"

class PlayerShip;

class PlayerShipMover : public PerFrameUpdateableObject {
private:
	PlayerShip& mParent;
	Vector2d mVelocity;
	InputListener mPointerInput;
	InputListener mShipInput;

public:
	PlayerShipMover(PlayerShip& in_parent);

	void update(double in_dt) override;
};

#endif