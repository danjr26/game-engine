#ifndef GAME_H
#define GAME_H

#include "pointer_input_context.h"
#include "player_ship_input_context.h"
#include "player_ship.h"
#include "collision_context.h"
#include "damage_manager.h"
#include "ai_target.h"

class Game {
public:
	using AITargetContainer = std::unordered_set<AITarget*>;

	enum CollisionFilters {
		player_ship = CollisionContext2d::Filters::user_defined,
		enemy_ship,
		player_bullet,
		enemy_bullet,
		solid
	};

private:
	PointerInputContext mPointerInput;
	player_ship::InputContext mPlayerShipInput;
	CollisionContext2d mMainCollisionContext;
	DamageManager mDamageManager;
	player_ship::PlayerShip* mPlayerShip;
	AITargetContainer mAITargets;

public:
	Game();
	~Game();

	void init();
	void preupdate(double in_dt);
	void postupdate(double in_dt);

	PointerInputContext& getPointerInput();
	player_ship::InputContext& getPlayerShipInput();
	player_ship::PlayerShip* getPlayerShip();
	CollisionContext2d& getMainCollisionContext();
	DamageManager& getDamageManager();
	AITargetContainer& getAITargets();
};

#endif