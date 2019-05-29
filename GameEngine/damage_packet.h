#ifndef DAMAGE_PACKET_H
#define DAMAGE_PACKET_H

struct DamagePacket {
	enum class Method {
		bulk,
		per_second
	} mMethod;
	float mAmount;
};

#endif