#ifndef PLAYER_SHIP_CAMERA_MOVER_H
#define PLAYER_SHIP_CAMERA_MOVER_H

#include "per_frame_updateable_object.h"
#include "input_listener.h"

namespace player_ship {
	class PlayerShip;

	class CameraMover {
	private:
		PlayerShip& mParent;
		InputListener mPointerInput;

	public:
		CameraMover(PlayerShip& i_parent);

		void update(double i_dt);
	};
}

#endif