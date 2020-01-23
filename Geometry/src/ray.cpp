#include "../include/internal/ray.h"
#include "../include/internal/line.h"

template<class T, uint n>
Ray<T, n>::Ray(const Vector<T, n>& i_point, const Vector<T, n>& i_direction) :
mPoint(i_point),
mDirection(i_direction.normalized()) 
{}

template<class T, uint n>
Ray<T, n>::Ray() :
mPoint(),
mDirection()
{}

template<class T, uint n>
Line<T, n> Ray<T, n>::toLine() const {
	return Line<T, n>::fromPointDirection(mPoint, mDirection);
}

template<class T, uint n>
void Ray<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	mPoint = i_transform.localToWorldPoint(mPoint);
	mDirection = i_transform.localToWorldDirection(mDirection);
}

template<class T, uint n>
void Ray<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	Vector<T, n> point = io_request.mPoint;
	T p = getProjectionCoefficient(point);
	T lowEnd = getProjectionCoefficient();

	if (p <= lowEnd) {
		if (io_request.wantsPoint()) io_request.mPoint = mPoint;
		if (io_request.wantsNormal()) io_request.mNormal = (point - mPoint).normalized();
		io_request.mContactType = ClosestRequest<T, n>::on_point;
	}
	else {
		if (io_request.wantsPoint() || io_request.wantsNormal()) {
			io_request.mPoint = getProjection(point);
		}
		if (io_request.wantsNormal()) {
			io_request.mNormal = (point - io_request.mPoint).normalized();
		}
		io_request.mContactType = ClosestRequest<T, n>::on_edge;
	}
}

template<class T, uint n>
Vector<T, n> Ray<T, n>::getPoint() const {
	return mPoint;
}

template<class T, uint n>
Vector<T, n> Ray<T, n>::getDirection() const {
	return mDirection;
}

template<class T, uint n>
T Ray<T, n>::getProjectionCoefficient() const {
	return mDirection.dot(mPoint);
}

template<class T, uint n>
T Ray<T, n>::getProjectionCoefficient(const Vector<T, n>& i_point) const {
	return (i_point - mPoint).projectionCoeff(mDirection);
}

template<class T, uint n>
Vector<T, n> Ray<T, n>::getProjection(const Vector<T, n>& i_point) const {
	return (i_point - mPoint).projection(mDirection) + mPoint;
}

template<class T, uint n>
Ray<T, n> Ray<T, n>::fromPointDirection(const Vector<T, n>& i_point, const Vector<T, n>& i_direction) {
	return Ray<T, n>(i_point, i_direction);
}

template<class T, uint n>
Ray<T, n> Ray<T, n>::fromPoints(const Vector<T, n>& i_point1, const Vector<T, n>& i_point2) {
	return Ray<T, n>(i_point1, i_point2 - i_point1);
}

template class Ray<float, 2>;
template class Ray<double, 2>;
template class Ray<float, 3>;
template class Ray<double, 3>;