#ifndef AI_TARGET_H
#define AI_TARGET_H

#include "allegiance.h"
#include "collider.h"

class AITarget {
public:
	virtual Collider2d const& getCollider() const = 0;
	virtual Vector2d getLinearVelocity() const = 0;
	virtual URotation2d getAngularVelocity() const = 0;
	virtual Allegiance getAllegiance() const = 0;
};

#endif