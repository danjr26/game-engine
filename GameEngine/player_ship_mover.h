#ifndef PLAYER_SHIP_MOVER_H
#define PLAYER_SHIP_MOVER_H

#include "per_frame_updateable_object.h"
#include "cubic_transitioner.h"
#include "vector.h"

class PlayerShip;

class PlayerShipMover : public PerFrameUpdateableObject {
private:
	PlayerShip& mParent;
	Vector2d mVelocity;

public:
	PlayerShipMover(PlayerShip& in_parent);

	void update(double in_dt) override;
};

#endif