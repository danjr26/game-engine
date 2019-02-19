#ifndef TEST_BULLET_H
#define TEST_BULLET_H

#include "sprite.h"
#include "deep_transformable_object.h"
#include "per_frame_updateable_object.h"
#include "destructable_object.h"
#include "basic_collision_mask.h"

class TestBullet : public DeepTransformableObject2d, public PerFrameUpdateableObject, 
	public DestructableObject {

private:
	Sprite mSprite;
	double mAge;

public:
	TestBullet();
	~TestBullet();

	virtual void update(double in_dt) override;

};

#endif