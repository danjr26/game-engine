#ifndef COLLISION_PARTNER_H
#define COLLISION_PARTNER_H

#include "collision.h"

template<class T, uint n>
class CollisionMask;

template<class T, uint n>
struct CollisionPartner {
	CollisionMask<T, n>* mMask;
	Collision<T, n> mCollision;

	bool operator==(const CollisionPartner& in_other) {
		return mMask == in_other.mMask;
	}
};

using CollisionPartner2f = CollisionPartner<float, 2>;
using CollisionPartner2d = CollisionPartner<double, 2>;
using CollisionPartner3f = CollisionPartner<float, 2>;
using CollisionPartner3d = CollisionPartner<double, 3>;

#endif