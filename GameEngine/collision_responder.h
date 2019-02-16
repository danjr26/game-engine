#ifndef COLLISION_RESPONDER_H
#define COLLISION_RESPONDER_H

#include "collision_packet.h"
#include "collision_context.h"

class CollisionResponder {
public:
	using Partner = typename CollisionContext2d::CollisionPartner;
	virtual void respond(const Partner& in_partner, CollisionPacket& out_packet) = 0;
};

#endif