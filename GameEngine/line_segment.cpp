#include "line_segment.h"
#include "exceptions.h"

template<class T, uint n>
inline LineSegment<T, n>::LineSegment(Vector<T, n>& in_point1, Vector<T, n>& in_point2) :
	mPoint1(in_point1),
	mPoint2(in_point2) {}

template<class T, uint n>
inline void LineSegment<T, n>::applyTransform(const Transform<T, n>& in_transform) {
	mPoint1 = in_transform.localToWorldPoint(mPoint1);
	mPoint2 = in_transform.localToWorldPoint(mPoint2);
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::getPoint1() const {
	return mPoint1;
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::getPoint2() const {
	return mPoint2;
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::getCenter() const {
	return (mPoint1 + mPoint2) / 2.0;
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::getOffset() const {
	return mPoint2 - mPoint1;
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::getDirection() const {
	return (mPoint2 - mPoint1).normalized();
}

template<class T, uint n>
inline T LineSegment<T, n>::getProjectionCoefficient1() const {
	return getDirection().dot(mPoint1);
}

template<class T, uint n>
inline T LineSegment<T, n>::getProjectionCoefficient2() const {
	return getDirection().dot(mPoint2);
}

template<class T, uint n>
inline T LineSegment<T, n>::getProjectionCoefficient(const Vector<T, n>& in_point) const {
	return getDirection().dot(in_point);
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::getProjection(const Vector<T, n>& in_point) const {
	return (in_point - mPoint1).projection(getDirection()) + mPoint1;
}

template<class T, uint n>
inline T LineSegment<T, n>::getLength() {
	return (mPoint2 - mPoint1).magnitude();
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::randomPoint() const {
	return mPoint1 + (mPoint2 - mPoint1) * GEUtil::random<T>(1);
}

template<class T, uint n>
inline LineSegment<T, n> LineSegment<T, n>::fromPoints(Vector<T, n>& in_point1, Vector<T, n>& in_point2) {
	return LineSegment<T, n>(in_point1, in_point2);
}

template<class T, uint n>
inline LineSegment<T, n> LineSegment<T, n>::fromPointOffset(Vector<T, n>& in_point, Vector<T, n>& in_offset) {
	return LineSegment<T, n>(in_point, in_point + in_offset);
}

template class LineSegment<float, 2>;
template class LineSegment<double, 2>;
template class LineSegment<float, 3>;
template class LineSegment<double, 3>;