#ifndef TEST_BULLET_COLLISION_RESPONDER_H
#define TEST_BULLET_COLLISION_RESPONDER_H

#include "collision_responder.h"

class TestBullet;

class TestBulletCollisionResponder : public CollisionResponder {
private:
	TestBullet& mParent;

public:
	TestBulletCollisionResponder(TestBullet& in_parent);

	void respond(const Partner& in_partner, CollisionPacket& out_packet) override;
};


#endif