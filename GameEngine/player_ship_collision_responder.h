#ifndef PLAYER_SHIP_COLLISION_RESPONDER_H
#define PLAYER_SHIP_COLLISION_RESPONDER_H

#include "collision_responder.h"

class PlayerShip;

class PlayerShipCollisionResponder : public CollisionResponder {
private:
	PlayerShip& mParent;

public:
	PlayerShipCollisionResponder(PlayerShip& in_parent);

	void respond(const Partner& in_partner, CollisionPacket& out_packet) override;
};

#endif