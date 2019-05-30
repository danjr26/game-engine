#ifndef DAMAGE_RECEIVER_H
#define DAMAGE_RECEIVER_H

#include "damage_packet.h"

class DamageReceiver {
public:
	virtual void receiveDamage(const DamagePacket& in_damage, double in_dt);
};

#endif