#ifndef COLLISION_PACKET_H
#define COLLISION_PACKET_H

#include "collision.h"
#include "definitions.h"
#include "located_vector.h"
#include "damage_packet.h"

struct CollisionPacket {
	LocatedVector2d mImpulse;
	DamagePacket mDamage;
};

#endif