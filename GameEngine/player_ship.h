#ifndef PLAYER_SHIP_H
#define PLAYER_SHIP_H

#include "player_ship_renderer.h"
#include "player_ship_mover.h"
#include "deep_transformable_object.h"

class PlayerShip : public DeepTransformableObject2d {
private:
	PlayerShipRenderer mRenderer;
	PlayerShipMover mMover;

public:
	PlayerShip();	
	PlayerShipRenderer& getRenderer();
	PlayerShipMover& getMover();
};

#endif