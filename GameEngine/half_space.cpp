#include "half_space.h"
#include "exceptions.h"

template<class T, uint n>
HalfSpace<T, n>::HalfSpace(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
	mPoint(in_point),
	mDirection(in_direction.normalized()) {}

template<class T, uint n>
HalfSpace<T, n>::HalfSpace() :
	mPoint(),
	mDirection()
{}

template<class T, uint n>
void HalfSpace<T, n>::applyTransform(const Transform<T, n>& in_transform) {
	mPoint = in_transform.localToWorldPoint(mPoint);
	mDirection = in_transform.localToWorldDirection(mDirection);
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::getPoint() const {
	return mPoint;
}

template<class T, uint n>
void HalfSpace<T, n>::setPoint(const Vector<T, n>& in_point) {
	mPoint = in_point;
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::getDirection() const {
	return mDirection;
}

template<class T, uint n>
void HalfSpace<T, n>::setDirection(const Vector<T, n>& in_direction) {
	mDirection = in_direction.normalized();
}

template<class T, uint n>
T HalfSpace<T, n>::getProjectionCoefficient() const {
	return mPoint.dot(mDirection);
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::getProjection() const {
	return mPoint.dot(mDirection) * mDirection;
}

template<class T, uint n>
HalfSpace<T, n> HalfSpace<T, n>::fromPointDirection(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
	return HalfSpace<T, n>(in_point, in_direction);
}

template class HalfSpace<float, 2>;
template class HalfSpace<double, 2>;
template class HalfSpace<float, 3>;
template class HalfSpace<double, 3>;