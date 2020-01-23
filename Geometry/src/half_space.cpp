#include "../include/internal/half_space.h"

template<class T, uint n>
HalfSpace<T, n>::HalfSpace(const Vector<T, n>& i_point, const Vector<T, n>& i_direction) :
	mPoint(i_point),
	mDirection(i_direction.normalized()) {}

template<class T, uint n>
HalfSpace<T, n>::HalfSpace() :
	mPoint(),
	mDirection()
{}

template<class T, uint n>
void HalfSpace<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	mPoint = i_transform.localToWorldPoint(mPoint);
	mDirection = i_transform.localToWorldDirection(mDirection);
}

template<class T, uint n>
void HalfSpace<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	if (io_request.wantsPoint()) {
		io_request.mPoint = io_request.mPoint.projection(mDirection) + mPoint;
	}

	if (io_request.wantsNormal()) {
		io_request.mNormal = -getDirection();
		io_request.mContactType = ClosestRequest<T, n>::on_edge;
	}

	if (n == 2) {
		io_request.mContactType = ClosestRequest<T, n>::on_edge;
	}
	else if (n == 3) {
		io_request.mContactType = ClosestRequest<T, n>::on_face;
	}
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::getPoint() const {
	return mPoint;
}

template<class T, uint n>
void HalfSpace<T, n>::setPoint(const Vector<T, n>& i_point) {
	mPoint = i_point;
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::getDirection() const {
	return mDirection;
}

template<class T, uint n>
void HalfSpace<T, n>::setDirection(const Vector<T, n>& i_direction) {
	mDirection = i_direction.normalized();
}

template<class T, uint n>
bool HalfSpace<T, n>::containsPoint(const Vector<T, n>& i_point) {
	return i_point.dot(mDirection) >= mPoint.dot(mDirection);
}

template<class T, uint n>
HalfSpace<T, n> HalfSpace<T, n>::fromPointDirection(const Vector<T, n>& i_point, const Vector<T, n>& i_direction) {
	return HalfSpace<T, n>(i_point, i_direction);
}

template class HalfSpace<float, 2>;
template class HalfSpace<double, 2>;
template class HalfSpace<float, 3>;
template class HalfSpace<double, 3>;