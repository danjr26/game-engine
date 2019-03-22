#ifndef PLAYER_SHIP_MOVER_H
#define PLAYER_SHIP_MOVER_H

#include "player_ship_input_context.h"
#include "pointer_input_context.h"
#include "per_frame_updateable_object.h"
#include "cubic_transitioner.h"
#include "vector.h"
#include "unclamped_rotation.h"

class PlayerShip;

class PlayerShipMover {
private:
	PlayerShip& mParent;
	InputListener mPointerInput;
	InputListener mShipInput;

public:
	PlayerShipMover(PlayerShip& in_parent);

	void update(double in_dt);
	void init();
};

#endif