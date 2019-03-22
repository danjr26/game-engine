#ifndef GAME_H
#define GAME_H

#include "pointer_input_context.h"
#include "player_ship_input_context.h"
#include "player_ship.h"
#include "collision_context.h"
#include "collision_response_manager.h"

class Game {
private:
	PointerInputContext mPointerInput;
	PlayerShipInputContext mPlayerShipInput;
	CollisionContext2d mMainCollisionContext;
	CollisionResponseManager mCollisionResponseManager;
	PlayerShip* mPlayerShip;

public:
	enum Allegiance {
		player,
		enemy,
		neutral
	};

	enum MainCollisionContextFilters {
		player_ship = CollisionContext2d::Filters::user_defined,
		enemy_ship,
		player_bullet,
		enemy_bullet
	};

public:
	Game();
	~Game();

	void Init();

	PointerInputContext& getPointerInput();
	PlayerShipInputContext& getPlayerShipInput();
	PlayerShip* getPlayerShip();
	CollisionContext2d& getMainCollisionContext();
	CollisionResponseManager& getCollisionResponseManager();
};

#endif