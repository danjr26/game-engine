#include "point_collider_basis.h"

template<class T, uint n>
void PointColliderBasis<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	*this = i_transform.localToWorldPoint(*this);
}

template<class T, uint n>
void PointColliderBasis<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	Vector<T, n> point = io_request.mPoint;
	if (io_request.wantsPoint()) io_request.mPoint = *this;
	if (io_request.wantsNormal()) io_request.mNormal = point - *this;
	io_request.mContactType = ClosestRequest<T, n>::on_point;
}