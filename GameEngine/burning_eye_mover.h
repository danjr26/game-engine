#ifndef TEST_ENEMY_MOVER_H
#define TEST_ENEMY_MOVER_H

#include "per_frame_updateable_object.h"
#include "hermite_transitioner.h"
#include "stepper.h"

namespace burning_eye {
	class BurningEye;

	class Mover {
	private:
		BurningEye& mParent;

	public:
		Mover(BurningEye& in_parent);
		~Mover();

		void update(double in_dt);
	};
}

#endif