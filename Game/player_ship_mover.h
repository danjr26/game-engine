#ifndef PLAYER_SHIP_MOVER_H
#define PLAYER_SHIP_MOVER_H

#include "player_ship_input_context.h"
#include "pointer_input_context.h"
#include "per_frame_updateable_object.h"
#include "cubic_transitioner.h"
#include "vector.h"
#include "unclamped_rotation.h"

namespace player_ship {
	class PlayerShip;

	class Mover {
	private:
		PlayerShip& mParent;
		InputListener mPointerInput;
		InputListener mShipInput;

	public:
		Mover(PlayerShip& i_parent);

		void update(double i_dt);
		void init();
	};
}
#endif