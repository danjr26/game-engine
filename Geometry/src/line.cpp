#include "../include/internal/line.h"

template<class T, uint n>
inline Line<T, n>::Line(const Vector<T, n>& i_point, const Vector<T, n> i_direction) :
	mPoint(i_point),
	mDirection(i_direction.normalized()) {}

template<class T, uint n>
Line<T, n>::Line() :
	mPoint(),
	mDirection()
{}

template<class T, uint n>
inline void Line<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	mPoint = i_transform.localToWorldPoint(mPoint);
	mDirection = i_transform.localToWorldDirection(mDirection);
}

template<class T, uint n>
void Line<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	Vector<T, n> point = io_request.mPoint;

	if (io_request.wantsPoint() || io_request.wantsNormal()) {
		io_request.mPoint = getProjection(point);
	}

	if (io_request.wantsNormal()) {
		io_request.mNormal = (point - io_request.mPoint).normalized();
	}

	io_request.mContactType = ClosestRequest<T, n>::on_edge;
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
T Line<T, n>::getProjectionCoeff() const {
	return mPoint.projectionCoeff(mDirection);
}

template<class T, uint n>
inline T Line<T, n>::getProjectionCoeff(const Vector<T, n>& i_point) const {
	return i_point.projectionCoeff(mDirection);
}

template<class T, uint n>
inline Vector<T, n> Line<T, n>::getProjection(const Vector<T, n>& i_point) const {
	return (i_point - mPoint).projection(mDirection) + mPoint;
}

template<class T, uint n>
T Line<T, n>::distanceTo(const Vector<T, n>& i_point) const {
	return (getProjection(i_point) - i_point).magnitude();
}

template<class T, uint n>
Vector<T, n> Line<T, n>::flip(const Vector<T, n>& i_point) const {
	Vector<T, n> projection = getProjection(i_point);
	return i_point + (projection - i_point) * 2;
}

template<class T, uint n>
inline Line<T, n> Line<T, n>::fromPoints(const Vector<T, n>& i_point1, const Vector<T, n>& i_point2) {
	return Line<T, n>(i_point1, i_point2 - i_point1);
}

template<class T, uint n>
inline Line<T, n> Line<T, n>::fromPointDirection(const Vector<T, n>& i_point, const Vector<T, n>& i_direction) {
	return Line<T, n>(i_point, i_direction);
}

template class Line<float, 2>;
template class Line<double, 2>;
template class Line<float, 3>;
template class Line<double, 3>;