#ifndef PLAYER_SHIP_INPUT_CONTEXT_H
#define PLAYER_SHIP_INPUT_CONTEXT_H

#include "input_context.h"
#include "definitions.h"

class PlayerShip;

class PlayerShipInputContext : public InputContext {
public:
	struct Actions {
		enum {
			count
		};
	};

	struct States {
		enum {
			fire_1,
			fire_2,
			fire_3,
			count
		};
	};

	struct Ranges {
		enum {
			move_x,
			move_y,
			count
		};
	};

public:
	PlayerShipInputContext();
};

#endif