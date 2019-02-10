#ifndef COLLISION_H
#define COLLISION_H

#include "vector.h"

template<class T, uint n>
struct Collision {
	Vector<T, n> mPoint;
	Vector<T, n> mSeparator;
	void* mOwner;
	bool mDid;

	Collision() :
		mPoint(),
		mSeparator(),
		mOwner(nullptr),
		mDid(false)
	{}
};

using Collision2f = Collision<float, 2>;
using Collision2d = Collision<double, 2>;

using Collision3f = Collision<float, 3>;
using Collision3d = Collision<double, 3>;

#endif