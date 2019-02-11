#include "line.h"
#include "exceptions.h"

template<class T, uint n>
inline Line<T, n>::Line(const Vector<T, n>& in_point, const Vector<T, n> in_direction) :
	mPoint(in_point),
	mDirection(in_direction.normalized()) {}

template<class T, uint n>
Line<T, n>::Line() :
	mPoint(),
	mDirection()
{}

template<class T, uint n>
inline void Line<T, n>::applyTransform(const Transform<T, n>& in_transform) {
	mPoint = in_transform.localToWorldPoint(mPoint);
	mDirection = in_transform.localToWorldDirection(mDirection);
}

template<class T, uint n>
inline Vector<T, n> Line<T, n>::getPoint() const {
	return mPoint;
}

template<class T, uint n>
inline Vector<T, n> Line<T, n>::getDirection() const {
	return mDirection;
}

template<class T, uint n>
T Line<T, n>::getProjectionCoefficient() const {
	return mPoint.projectionCoeff(mDirection);
}

template<class T, uint n>
inline T Line<T, n>::getProjectionCoefficient(const Vector<T, n>& in_point) const {
	return in_point.projectionCoeff(mDirection);
}

template<class T, uint n>
inline Vector<T, n> Line<T, n>::getProjection(const Vector<T, n>& in_point) const {
	return (in_point - mPoint).projection(mDirection) + mPoint;
}

template<class T, uint n>
T Line<T, n>::distanceTo(const Vector<T, n>& in_point) const {
	return (getProjection(in_point) - in_point).magnitude();
}

template<class T, uint n>
Vector<T, n> Line<T, n>::flip(const Vector<T, n>& in_point) const {
	Vector<T, n> projection = getProjection(in_point);
	return in_point + (projection - in_point) * 2;
}

template<class T, uint n>
inline Line<T, n> Line<T, n>::fromPoints(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2) {
	return Line<T, n>(in_point1, in_point2 - in_point1);
}

template<class T, uint n>
inline Line<T, n> Line<T, n>::fromPointDirection(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
	return Line<T, n>(in_point, in_direction);
}

template class Line<float, 2>;
template class Line<double, 2>;
template class Line<float, 3>;
template class Line<double, 3>;