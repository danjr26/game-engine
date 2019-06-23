#ifndef PLAYER_SHIP_TARGET_H
#define PLAYER_SHIP_TARGET_H


#include "ai_target.h"

class PlayerShip;

class PlayerShipTarget : public AITarget {
private:
	PlayerShip& mParent;

public:
	PlayerShipTarget(PlayerShip& in_parent);
	~PlayerShipTarget();

	virtual CollisionMask2d const& getCollisionMask() const override;
	virtual Vector2d getLinearVelocity() const override;
	virtual URotation2d getAngularVelocity() const override;
	virtual Allegiance getAllegiance() const override;
};

#endif