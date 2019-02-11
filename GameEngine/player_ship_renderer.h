#ifndef PLAYER_SHIP_RENDERER_H
#define PLAYER_SHIP_RENDERER_H

#include "sprite.h"

class PlayerShip;

class PlayerShipRenderer {
private:
	PlayerShip& mParent;
	Sprite mSprite;
public:
	PlayerShipRenderer(PlayerShip& in_parent);
	~PlayerShipRenderer();
};

#endif