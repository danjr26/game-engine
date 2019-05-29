#ifndef RAYCAST_H
#define RAYCAST_H

#include <limits>
#include <functional>
#include "ray.h"
#include "collision_context.h"
#include "filter_query.h"
#include "basic_collision_mask.h"

template<class T, uint n>
bool raycast(
	Ray<T, n>& in_ray,
	CollisionContext<T, n>& in_context,
	typename CollisionContext<T, n>::CollisionPartner& out_partner,
	std::function<bool(CollisionMask<T, n>*)> in_filter) {

	BasicCollisionMask<Ray<T, n>, T, n> rayMask(in_ray);
	std::vector<typename CollisionContext<T, n>::CollisionPartner> partners;
	in_context.check(&rayMask, partners, in_filter);
	if (partners.empty()) return false;
	T minDistance = std::numeric_limits<T>::infinity();
	for (auto it = partners.begin(); it != partners.end(); it++) {
		T distance = (it->mCollision.mPoint - in_ray.getPoint()).magnitude();
		if (distance < minDistance) {
			minDistance = distance;
			out_partner = *it;
		}
	}
	return true;
}

#endif


