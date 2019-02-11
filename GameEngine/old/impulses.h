#ifndef IMPULSES_H
#define IMPULSES_H

#include "component.h"

class Impulse : public Component {
public:
	AIShip* parent;

					Impulse	(AIShip* parent);
	virtual			~Impulse();
	virtual Vector3	Get		();
};

class ConstImpulse : public Impulse {
public:
	Vector3 v;
	bool subjective;

					ConstImpulse(AIShip* parent, Vector3 v, bool subjective);
	virtual			~ConstImpulse();
	virtual Vector3 Get();
};

enum ProxImpulseType {
	IMPULSE_ALL,
	IMPULSE_ALLEGIANCE,
	IMPULSE_TYPE,
	IMPULSE_ID
};

class PolyProxImpulse : public Impulse {
public:
	float coeff;
	float exp;

					PolyProxImpulse	(AIShip* parent, float coeff, float exp);
	virtual			~PolyProxImpulse();
	virtual Vector3 Get				();
};
#endif
