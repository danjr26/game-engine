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

private:
	PlayerShip& mParent;

public:
	PlayerShipInputContext(PlayerShip& in_parent);
};

#endif