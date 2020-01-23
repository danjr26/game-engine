#ifndef COLLISION_PARTNER_H
#define COLLISION_PARTNER_H

#include "collision.h"

template<class T, uint n>
class Collider;

template<class T, uint n>
struct CollisionPartner {
	Collider<T, n>* mMask;
	Collision<T, n> mCollision;

	CollisionPartner() :
		mMask(nullptr),
		mCollision()
	{}

	CollisionPartner(Collider<T, n>* i_mask, Collision<T, n> i_collision) :
		mMask(i_mask),
		mCollision(i_collision)
	{}

	bool operator==(const CollisionPartner& i_other) {
		return mMask == i_other.mMask;
	}
};

using CollisionPartner2f = CollisionPartner<float, 2>;
using CollisionPartner2d = CollisionPartner<double, 2>;
using CollisionPartner3f = CollisionPartner<float, 2>;
using CollisionPartner3d = CollisionPartner<double, 3>;

#endif