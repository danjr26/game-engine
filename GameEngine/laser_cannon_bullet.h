#ifndef LASER_CANNON_BULLET_H
#define LASER_CANNON_BULLET_H

#include "sprite.h"
#include "deep_transformable_object.h"
#include "per_frame_updateable_object.h"
#include "destructable_object.h"
#include "basic_collision_mask.h"
#include "rigid_body.h"
#include "collision_responder.h"

class LaserCannonBullet : public DeepTransformableObject2d, public PerFrameUpdateableObject,
	public DestructableObject {

private:
	CollisionResponder mCollisionResponder;
	RigidBody2 mRigidBody;
	Sprite mSprite;
	double mAge;
	double mLifeSpan;

public:
	LaserCannonBullet();
	~LaserCannonBullet();

	virtual void update(double in_dt) override;

	CollisionMask2d& getCollisionMask();
};

#endif