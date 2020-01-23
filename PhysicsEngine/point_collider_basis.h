#ifndef POINT_COLLIDER_BASIS_H
#define POINT_COLLIDER_BASIS_H

#include "vector.h"
#include "collider_basis.h"

template<class T, uint n>
class PointColliderBasis : 
	public Vector<T, n>, 
	public ColliderBasis<T, n> 
{
	void applyTransform(const Transform<T, n>& i_transform) override;
	void getClosest(ClosestRequest<T, n>& io_request) const override;
};

#endif

