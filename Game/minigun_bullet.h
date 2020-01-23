#ifndef MINIGUN_BULLET_H
#define MINIGUN_BULLET_H

#include "sprite.h"
#include "deep_transformable_object.h"
#include "per_frame_updateable_object.h"
#include "destructable_object.h"
#include "basic_collider.h"
#include "rigid_body.h"
#include "collision_responder.h"
#include "collision_queue.h"

class MinigunBullet : 
	public DeepTransformableObject2d, 
	public PerFrameUpdateableObject,
	public DestructableObject {

private:
	CollisionQueue2d mCollisionQueue;
	RigidBody2 mRigidBody;
	Sprite mSprite;
	double mAge;
	double mLifeSpan;

public:
	MinigunBullet();
	~MinigunBullet();

	void update(double i_dt) override;

	Collider2d& getCollider();
};

#endif