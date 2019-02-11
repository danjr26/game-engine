#include "player_ship.h"

PlayerShip::PlayerShip() :
	mRenderer(*this),
	mMover(*this)
{}

PlayerShipRenderer& PlayerShip::getRenderer() {
	return mRenderer;
}

PlayerShipMover& PlayerShip::getMover() {
	return mMover;
}
