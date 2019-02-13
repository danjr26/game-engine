#include "game.h"
#include "game_engine.h"
#include "player_ship.h"

Game::Game() :
	mPointerInput(),
	mPlayerShipInput() {

	GE.input().add(&mPointerInput);
	GE.input().add(&mPlayerShipInput);
}

void Game::Init() {
	PlayerShip* playerShip = new PlayerShip();
	playerShip->getTransform().translateWorld(Vector2d(3, 3));
	playerShip->getTransform().rotateLocal(Rotation2d(PI / 4));
}

PointerInputContext& Game::getPointerInput() {
	return mPointerInput;
}

PlayerShipInputContext& Game::getPlayerShipInput() {
	return mPlayerShipInput;
}
