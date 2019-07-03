#ifndef TEST_ENEMY_RENDERER_H
#define TEST_ENEMY_RENDERER_H

#include "circle_renderer.h"
#include "particle_system2.h"
#include "particle_system2_specifiers.h"


class BurningEye;

class BurningEyeRenderer {
private:
	BurningEye& mParent;
	CircleRenderer mSprite;
	ParticleSystem2* mIris;
	ParticleSystem2* mPupil;

public:
	BurningEyeRenderer(BurningEye& in_parent);
	~BurningEyeRenderer();

	void init();
};

#endif