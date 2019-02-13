#include "game.h"
#include "game_engine.h"
#include "player_ship.h"
#include "test_enemy.h"

Game::Game() :
	mPointerInput(),
	mPlayerShipInput(),
	mPlayerShip(nullptr) {

	GE.input().add(&mPointerInput);
	GE.input().add(&mPlayerShipInput);
}

void Game::Init() {
	mPlayerShip = new PlayerShip();
	//mPlayerShip->getTransform().translateWorld(Vector2d(3, 3));
	//mPlayerShip->getTransform().rotateLocal(Rotation2d(PI / 4));

	TestEnemy* testEnemy = new TestEnemy();
	testEnemy->getTransform().setLocalPosition(Vector2d(4, 4));
}

PointerInputContext& Game::getPointerInput() {
	return mPointerInput;
}

PlayerShipInputContext& Game::getPlayerShipInput() {
	return mPlayerShipInput;
}

PlayerShip* Game::getPlayerShip() {
	return mPlayerShip;
}
