#ifndef PLAYER_SHIP_CAMERA_MOVER_H
#define PLAYER_SHIP_CAMERA_MOVER_H

#include "per_frame_updateable_object.h"

class PlayerShip;

class PlayerShipCameraMover {
private:
	PlayerShip& mParent;

public:
	PlayerShipCameraMover(PlayerShip& in_parent);

	void update(double in_dt);
};

#endif