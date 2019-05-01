#include "player_ship_collision_responder.h"

PlayerShipCollisionResponder::PlayerShipCollisionResponder(PlayerShip& in_parent) :
	mParent(in_parent)
{}

void PlayerShipCollisionResponder::respond(const Partner& in_partner, CollisionPacket& out_packet) {
	out_packet.mDamage.mAmount = 0.0f;
	out_packet.mImpulse.mPosition = in_partner.mCollision.mPoint;
	out_packet.mImpulse.mVector = Vector2d();
}
