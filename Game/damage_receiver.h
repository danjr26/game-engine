#ifndef DAMAGE_RECEIVER_H
#define DAMAGE_RECEIVER_H

#include "damage_packet.h"

class DamageReceiver {
public:
	virtual void receiveDamage(const DamagePacket& i_damage, double i_dt);
};

#endif