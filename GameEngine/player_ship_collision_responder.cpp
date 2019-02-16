#include "player_ship_collision_responder.h"

PlayerShipCollisionResponder::PlayerShipCollisionResponder(PlayerShip& in_parent) :
	mParent(in_parent)
{}

void PlayerShipCollisionResponder::respond(const Partner& in_partner, CollisionPacket& out_packet) {
	
}
