#ifndef GAME_H
#define GAME_H

#include "pointer_input_context.h"
#include "player_ship_input_context.h"
#include "player_ship.h"

class Game {
private:
	PointerInputContext mPointerInput;
	PlayerShipInputContext mPlayerShipInput;
	PlayerShip* mPlayerShip;

public:
	Game();

	void Init();

	PointerInputContext& getPointerInput();
	PlayerShipInputContext& getPlayerShipInput();
	PlayerShip* getPlayerShip();
};

#endif