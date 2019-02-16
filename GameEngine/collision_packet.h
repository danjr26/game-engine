#ifndef COLLISION_PACKET_H
#define COLLISION_PACKET_H

#include "collision.h"
#include "definitions.h"
#include "located_vector.h"

struct CollisionPacket {
	enum Message : uint {

	};

	uint message;
	LocatedVector2d impulse;
	float damage;
};

#endif