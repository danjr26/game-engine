#ifndef COLLISION_H
#define COLLISION_H

#include "vector.h"

using collision_flags_t = uint;

template<class T, uint n>
struct Collision {
	enum Fields : collision_flags_t {
		point = 1 << 0,
		separator = 1 << 1
	};

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

	operator bool() const {
		return mDid;
	}

	bool operator!() const {
		return !mDid;
	}

	Collision<T, n>& operator=(bool i_value) {
		mDid = i_value;
	}
};

using Collision2f = Collision<float, 2>;
using Collision2d = Collision<double, 2>;

using Collision3f = Collision<float, 3>;
using Collision3d = Collision<double, 3>;

#endif