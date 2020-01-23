#ifndef TEST_ENEMY_RENDERER_H
#define TEST_ENEMY_RENDERER_H

#include "circle_renderer.h"
#include "particle_system2.h"
#include "particle_system2_specifiers.h"

namespace burning_eye {
	class BurningEye;

	class Renderer {
	private:
		BurningEye& mParent;
		CircleRenderer mSprite;
		ParticleSystem2* mIris;
		ParticleSystem2* mPupil;

	public:
		Renderer(BurningEye& i_parent);
		~Renderer();

		void init();
	};
}

#endif