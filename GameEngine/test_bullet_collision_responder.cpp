#include "test_bullet_collision_responder.h"
#include "test_bullet.h"

TestBulletCollisionResponder::TestBulletCollisionResponder(TestBullet& in_parent) :
	mParent(in_parent)
{}

void TestBulletCollisionResponder::respond(const Partner& in_partner, CollisionPacket& out_packet) {
	
}
