#ifndef WEAPON_H
#define WEAPON_H

#include "deep_transformable_object.h"

class Weapon : public DeepTransformableObject2d {
public:
	virtual void update(double in_dt) = 0;
	virtual void startFire() = 0;
	virtual void endFire() = 0;
};

#endif