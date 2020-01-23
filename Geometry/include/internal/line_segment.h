#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "geometric.h"

template<class T, uint n>
class Line;

template<class T, uint n>
class Ray;

template<class T, uint n>
class LineSegment : public Geometric<T, n> {
private:
	Vector<T, n> mPoint1;
	Vector<T, n> mPoint2;

	LineSegment(const Vector<T, n>& i_point1, const Vector<T, n>& i_point2);

public:
	LineSegment();

	Line<T, n> toLine() const;
	Ray<T, n> toRay() const;

	void applyTransform(const Transform<T, n>& i_transform) override;
	void getClosest(ClosestRequest<T, n>& io_request) const override;

	Vector<T, n> getPoint1() const;
	Vector<T, n> getPoint2() const;
	Vector<T, n> getCenter() const;
	Vector<T, n> getOffset() const;
	Vector<T, n> getDirection() const;
	T getProjectionCoefficient1() const;
	T getProjectionCoefficient2() const;
	T getProjectionCoefficient(const Vector<T, n>& i_point) const;
	Vector<T, n> getProjection(const Vector<T, n>& i_point) const;
	T getLength();

	Vector<T, n> randomPoint() const;

public:
	static LineSegment<T, n> fromPoints(const Vector<T, n>& i_point1, const Vector<T, n>& i_point2);
	static LineSegment<T, n> fromPointOffset(const Vector<T, n>& i_point, const Vector<T, n>& i_offset);
};

using LineSegment2f = LineSegment<float, 2>;
using LineSegment2d = LineSegment<double, 2>;
using LineSegment3f = LineSegment<float, 3>;
using LineSegment3d = LineSegment<double, 3>;

#endif


