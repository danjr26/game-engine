#include "player_ship.h"
#include "game_engine.h"

PlayerShip::PlayerShip() :
	mRenderer(*this),
	mMover(*this),
	mInput(*this) {

	GE.perFrameUpdate().add(&mMover);
	GE.input().add(&mInput);
}

PlayerShipRenderer& PlayerShip::getRenderer() {
	return mRenderer;
}

PlayerShipMover& PlayerShip::getMover() {
	return mMover;
}

PlayerShipInputContext& PlayerShip::getInput() {
	return mInput;
}
