#ifndef TEST_BULLET_H
#define TEST_BULLET_H

#include "sprite.h"
#include "deep_transformable_object.h"
#include "per_frame_updateable_object.h"

class TestBullet : public DeepTransformableObject2d, public PerFrameUpdateableObject {
private:
	Sprite mSprite;
	double age;

public:
	TestBullet();

	virtual void update(double in_dt) override;

};

#endif