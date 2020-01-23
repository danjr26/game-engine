#include "../include/internal/line_segment.h"
#include "../include/internal/line.h"
#include "../include/internal/ray.h"

template<class T, uint n>
LineSegment<T, n>::LineSegment(const Vector<T, n>& i_point1, const Vector<T, n>& i_point2) :
	mPoint1(i_point1),
	mPoint2(i_point2) {}

template<class T, uint n>
LineSegment<T, n>::LineSegment() :
	mPoint1(),
	mPoint2()
{}

template<class T, uint n>
Line<T, n> LineSegment<T, n>::toLine() const {
	return Line<T, n>::fromPoints(mPoint1, mPoint2);
}

template<class T, uint n>
Ray<T, n> LineSegment<T, n>::toRay() const {
	return Ray<T, n>::fromPoints(mPoint1, mPoint2);
}

template<class T, uint n>
void LineSegment<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	mPoint1 = i_transform.localToWorldPoint(mPoint1);
	mPoint2 = i_transform.localToWorldPoint(mPoint2);
}

template<class T, uint n>
void LineSegment<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	Vector<T, n> point = io_request.mPoint;
	T p = getProjectionCoefficient(point);
	T lowEnd = getProjectionCoefficient1();
	T highEnd = getProjectionCoefficient2();

	if (p <= lowEnd) {
		if (io_request.wantsPoint()) io_request.mPoint = mPoint1;
		if (io_request.wantsNormal()) io_request.mNormal = (point - mPoint1).normalized();
		io_request.mContactType = ClosestRequest<T, n>::on_point;
	}
	else if (p >= highEnd) {
		if (io_request.wantsPoint()) io_request.mPoint = mPoint2;
		if (io_request.wantsNormal()) io_request.mNormal = (point - mPoint2).normalized();
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
Vector<T, n> LineSegment<T, n>::getPoint1() const {
	return mPoint1;
}

template<class T, uint n>
Vector<T, n> LineSegment<T, n>::getPoint2() const {
	return mPoint2;
}

template<class T, uint n>
Vector<T, n> LineSegment<T, n>::getCenter() const {
	return (mPoint1 + mPoint2) / 2.0;
}

template<class T, uint n>
Vector<T, n> LineSegment<T, n>::getOffset() const {
	return mPoint2 - mPoint1;
}

template<class T, uint n>
Vector<T, n> LineSegment<T, n>::getDirection() const {
	return (mPoint2 - mPoint1).normalized();
}

template<class T, uint n>
T LineSegment<T, n>::getProjectionCoefficient1() const {
	return getDirection().dot(mPoint1);
}

template<class T, uint n>
T LineSegment<T, n>::getProjectionCoefficient2() const {
	return getDirection().dot(mPoint2);
}

template<class T, uint n>
T LineSegment<T, n>::getProjectionCoefficient(const Vector<T, n>& i_point) const {
	return getDirection().dot(i_point);
}

template<class T, uint n>
Vector<T, n> LineSegment<T, n>::getProjection(const Vector<T, n>& i_point) const {
	return (i_point - mPoint1).projection(getDirection()) + mPoint1;
}

template<class T, uint n>
T LineSegment<T, n>::getLength() {
	return (mPoint2 - mPoint1).magnitude();
}

template<class T, uint n>
Vector<T, n> LineSegment<T, n>::randomPoint() const {
	return mPoint1 + (mPoint2 - mPoint1) * random<T>(1);
}

template<class T, uint n>
LineSegment<T, n> LineSegment<T, n>::fromPoints(const Vector<T, n>& i_point1, const Vector<T, n>& i_point2) {
	return LineSegment<T, n>(i_point1, i_point2);
}

template<class T, uint n>
LineSegment<T, n> LineSegment<T, n>::fromPointOffset(const Vector<T, n>& i_point, const Vector<T, n>& i_offset) {
	return LineSegment<T, n>(i_point, i_point + i_offset);
}

template class LineSegment<float, 2>;
template class LineSegment<double, 2>;
template class LineSegment<float, 3>;
template class LineSegment<double, 3>;