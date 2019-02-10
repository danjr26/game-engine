#include "ray.h"
#include "exceptions.h"

template<class T, uint n>
inline Ray<T, n>::Ray(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
	mPoint(in_point),
	mDirection(in_direction.normalized()) {}

template<class T, uint n>
inline void Ray<T, n>::applyTransform(const Transform<T, n>& in_transform) {
	mPoint = in_transform.localToWorldPoint(mPoint);
	mDirection = in_transform.localToWorldDirection(mDirection);
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::getPoint() const {
	return mPoint;
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::getDirection() const {
	return mDirection;
}

template<class T, uint n>
inline T Ray<T, n>::getProjectionCoefficient() const {
	return mDirection.dot(mPoint);
}

template<class T, uint n>
inline T Ray<T, n>::getProjectionCoefficient(const Vector<T, n>& in_point) const {
	return (in_point - mPoint).projectionCoeff(mDirection);
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::getProjection(const Vector<T, n>& in_point) const {
	return (in_point - mPoint).projection(mDirection) + mPoint;
}

template<class T, uint n>
inline Ray<T, n> Ray<T, n>::fromPointDirection(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
	return Ray<T, n>(in_point, in_direction);
}

template<class T, uint n>
inline Ray<T, n> Ray<T, n>::fromPoints(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2) {
	return Ray<T, n>(in_point1, in_point2 - in_point1);
}

template class Ray<float, 2>;
template class Ray<double, 2>;
template class Ray<float, 3>;
template class Ray<double, 3>;