#ifndef PLAYER_SHIP_TARGET_H
#define PLAYER_SHIP_TARGET_H


#include "ai_target.h"

namespace player_ship {
	class PlayerShip;

	class Target : public AITarget {
	private:
		PlayerShip& mParent;

	public:
		Target(PlayerShip& i_parent);
		~Target();

		virtual Collider2d const& getCollider() const override;
		virtual Vector2d getLinearVelocity() const override;
		virtual URotation2d getAngularVelocity() const override;
		virtual Allegiance getAllegiance() const override;
	};
}

#endif