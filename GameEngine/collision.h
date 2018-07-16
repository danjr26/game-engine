#ifndef COLLISION_H
#define COLLISION_H

#include "vector.h"

template<class T>
struct Collision {
	bool didCollide;
	union {
		Vector<T, 3> collisionPoint3;
		struct {
			Vector<T, 2> collisionPoint2;
			T padding;
		};
	};
	T collisionTime;

	Collision() :
		didCollide(false),
		collisionPoint3(),
		collisionTime(0)
	{}
};

using Collisionf = Collision<float>;
using Collisiond = Collision<double>;

#endif