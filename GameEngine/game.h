#ifndef GAME_H
#define GAME_H

#include "pointer_input_context.h"
#include "player_ship_input_context.h"

class Game {
private:
	PointerInputContext mPointerInput;
	PlayerShipInputContext mPlayerShipInput;


public:
	Game();

	void Init();

	PointerInputContext& getPointerInput();
	PlayerShipInputContext& getPlayerShipInput();
};

#endif