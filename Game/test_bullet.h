#ifndef TEST_BULLET_H
#define TEST_BULLET_H

#include "sprite.h"
#include "deep_transformable_object.h"
#include "per_frame_updateable_object.h"
#include "destructable_object.h"
#include "basic_collider.h"
#include "rigid_body.h"
#include "collision_responder.h"

class TestBullet : 
	public DeepTransformableObject2d, 
	public PerFrameUpdateableObject, 
	public DestructableObject {

private:
	CollisionResponder mCollisionResponder;
	RigidBody2 mRigidBody;
	Sprite mSprite;
	double mAge;

public:
	TestBullet();
	~TestBullet();

	virtual void update(double i_dt) override;

	Collider2d& getCollider();
};

#endif