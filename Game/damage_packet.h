#ifndef DAMAGE_PACKET_H
#define DAMAGE_PACKET_H

#include "located_vector.h"

struct DamagePacket {
	enum class Method {
		bulk,
		per_second
	} mMethod;
	float mAmount;
	LocatedVector2d mImpulse;
};

#endif