#ifndef PLAYER_SHIP_WEAPONS_SYSTEM_H
#define PLAYER_SHIP_WEAPONS_SYSTEM_H

#include "per_frame_updateable_object.h"
#include "weapon.h"
#include "input_listener.h"

namespace player_ship {
	class PlayerShip;

	class WeaponsSystem {
	private:
		PlayerShip& mParent;
		Weapon* mActivePrimary;
		Weapon* mActiveSecondary;
		InputListener mShipInput;

	public:
		WeaponsSystem(PlayerShip& i_parent);

		virtual void update(double i_dt);

		Weapon* getPrimary();
		Weapon* getSecondary();
		void setPrimary(Weapon* i_weapon);
		void setSecondary(Weapon* i_weapon);
	};
}

#endif