#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "transform.h"

template<class T, uint n>
class LineSegment {
private:
	Vector<T, n> mPoint1;
	Vector<T, n> mPoint2;

	LineSegment(Vector<T, n>& in_point1, Vector<T, n>& in_point2);

public:
	LineSegment();

	void applyTransform(const Transform<T, n>& in_transform);

	Vector<T, n> getPoint1() const;
	Vector<T, n> getPoint2() const;
	Vector<T, n> getCenter() const;
	Vector<T, n> getOffset() const;
	Vector<T, n> getDirection() const;
	T getProjectionCoefficient1() const;
	T getProjectionCoefficient2() const;
	T getProjectionCoefficient(const Vector<T, n>& in_point) const;
	Vector<T, n> getProjection(const Vector<T, n>& in_point) const;
	T getLength();

	Vector<T, n> randomPoint() const;

public:
	static LineSegment<T, n> fromPoints(Vector<T, n>& in_point1, Vector<T, n>& in_point2);
	static LineSegment<T, n> fromPointOffset(Vector<T, n>& in_point, Vector<T, n>& in_offset);
};

using LineSegment2f = LineSegment<float, 2>;
using LineSegment2d = LineSegment<double, 2>;
using LineSegment3f = LineSegment<float, 3>;
using LineSegment3d = LineSegment<double, 3>;

#endif


