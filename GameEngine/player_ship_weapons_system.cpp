#include "player_ship_weapons_system.h"

PlayerShipWeaponsSystem::PlayerShipWeaponsSystem(PlayerShip& in_parent) :
	mParent(in_parent),
	mActivePrimary(nullptr),
	mActiveSecondary(nullptr)
{}

void PlayerShipWeaponsSystem::update(double in_dt) {
	
}

Weapon* PlayerShipWeaponsSystem::getPrimary() {
	return mActivePrimary;
}

Weapon* PlayerShipWeaponsSystem::getSecondary() {
	return mActiveSecondary;
}

void PlayerShipWeaponsSystem::setPrimary(Weapon* in_weapon) {
	mActivePrimary = in_weapon;
}

void PlayerShipWeaponsSystem::setSecondary(Weapon* in_weapon) {
	mActiveSecondary = in_weapon;
}
