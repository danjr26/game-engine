#ifndef PLAYER_SHIP_RENDERER_H
#define PLAYER_SHIP_RENDERER_H

#include "sprite.h"

namespace player_ship {
	class PlayerShip;

	class Renderer {
	private:
		PlayerShip& mParent;
		Sprite mSprite;
	public:
		Renderer(PlayerShip& i_parent);
		~Renderer();

		void init();
	};
}

#endif