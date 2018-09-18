#ifndef COLLISION_H
#define COLLISION_H

#include "vector.h"

template<class T, uint n>
struct Collision {
	bool didCollide;
	Vector<T, n> collisionPoint;
	T collisionTime;

	Collision() :
		didCollide(false),
		collisionPoint(),
		collisionTime(0)
	{}
};

using Collision2f = Collision<float, 2>;
using Collision2d = Collision<double, 2>;

using Collision3f = Collision<float, 3>;
using Collision3d = Collision<double, 3>;

#endif