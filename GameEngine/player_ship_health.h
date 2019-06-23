#ifndef PLAYER_SHIP_HEALTH_H
#define PLAYER_SHIP_HEALTH_H

#include "damage_receiver.h"

class PlayerShip;

class PlayerShipHealth : public DamageReceiver {
private:
	PlayerShip& mParent;
	float mHealth;

public:
	PlayerShipHealth(PlayerShip& in_parent);
	void receiveDamage(const DamagePacket& in_damage, double in_dt) override;
	bool isDead() const;
};

#endif