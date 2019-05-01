#include "test_bullet_collision_responder.h"
#include "test_bullet.h"

TestBulletCollisionResponder::TestBulletCollisionResponder(TestBullet& in_parent) :
	mParent(in_parent)
{}

void TestBulletCollisionResponder::respond(const Partner& in_partner, CollisionPacket& out_packet) {
	out_packet.mDamage.mAmount = 2.0;
	out_packet.mImpulse.mPosition = in_partner.mCollision.mPoint;
	out_packet.mImpulse.mVector = Vector2d();
}
