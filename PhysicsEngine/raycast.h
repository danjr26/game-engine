#ifndef RAYCAST_H
#define RAYCAST_H

#include <limits>
#include <functional>
#include "ray.h"
#include "collision_context.h"
#include "filter_query.h"
#include "basic_collider.h"

template<class T, uint n>
bool raycast(
	Ray<T, n>& i_ray,
	CollisionContext<T, n>& i_context,
	CollisionPartner<T, n>& o_partner,
	std::function<bool(Collider<T, n>*)> i_filter) {

	BasicCollider<Ray<T, n>, T, n> rayMask(i_ray);
	std::vector<CollisionPartner<T, n>> partners;
	i_context.check(&rayMask, partners, i_filter);
	if (partners.empty()) return false;
	T minDistance = std::numeric_limits<T>::infinity();
	for (auto it = partners.begin(); it != partners.end(); it++) {
		T distance = (it->mCollision.mPoint - i_ray.getPoint()).magnitude();
		if (distance < minDistance) {
			minDistance = distance;
			o_partner = *it;
		}
	}
	return true;
}

#endif


