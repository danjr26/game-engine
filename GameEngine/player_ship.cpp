#include "player_ship.h"
#include "game_engine.h"

PlayerShip::PlayerShip() :
	mRenderer(*this),
	mMover(*this) {

	GE.perFrameUpdate().add(&mMover);
}

PlayerShipRenderer& PlayerShip::getRenderer() {
	return mRenderer;
}

PlayerShipMover& PlayerShip::getMover() {
	return mMover;
}


