#ifndef GAME_H
#define GAME_H

#include "pointer_input_context.h"
#include "player_ship_input_context.h"
#include "player_ship.h"
#include "collision_context.h"

class Game {
private:
	PointerInputContext mPointerInput;
	PlayerShipInputContext mPlayerShipInput;
	CollisionContext2d mMainCollisionContext;
	PlayerShip* mPlayerShip;

public:
	enum Allegiance {
		player,
		enemy,
		neutral
	};

	enum CollisionFilters {
		player_ship = CollisionContext2d::Filters::user_defined,
		enemy_ship,
		player_bullet,
		enemy_bullet
	};

public:
	Game();
	~Game();

	void init();

	PointerInputContext& getPointerInput();
	PlayerShipInputContext& getPlayerShipInput();
	PlayerShip* getPlayerShip();
	CollisionContext2d& getMainCollisionContext();
};

#endif