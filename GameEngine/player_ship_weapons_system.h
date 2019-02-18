#ifndef PLAYER_SHIP_WEAPONS_SYSTEM_H
#define PLAYER_SHIP_WEAPONS_SYSTEM_H

#include "per_frame_updateable_object.h"
#include "weapon.h"
#include "input_listener.h"

class PlayerShip;

class PlayerShipWeaponsSystem {
private:
	PlayerShip& mParent;
	Weapon* mActivePrimary;
	Weapon* mActiveSecondary;
	InputListener mShipInput;

public:
	PlayerShipWeaponsSystem(PlayerShip& in_parent);

	virtual void update(double in_dt);

	Weapon* getPrimary();
	Weapon* getSecondary();
	void setPrimary(Weapon* in_weapon);
	void setSecondary(Weapon* in_weapon);
};

#endif