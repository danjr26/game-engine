#ifndef COLLISION_H
#define COLLISION_H

#include "vector.h"

template<class T, uint n>
struct Collision {
	Vector<T, n> point;
	Vector<T, n> separator;
	void* owner;
	bool did;

	Collision() :
		point(),
		separator(),
		owner(nullptr),
		did(false)
	{}
};

using Collision2f = Collision<float, 2>;
using Collision2d = Collision<double, 2>;

using Collision3f = Collision<float, 3>;
using Collision3d = Collision<double, 3>;

#endif