#ifndef LINE_H
#define LINE_H

#include "transform.h"

template<class T, uint n>
class Line {
private:
	Vector<T, n> mPoint;
	Vector<T, n> mDirection;

private:
	Line(const Vector<T, n>& in_point, const Vector<T, n> in_direction);

public:
	Line();

	void applyTransform(const Transform<T, n>& in_transform);
	Vector<T, n> getPoint() const;
	Vector<T, n> getDirection() const;
	T getProjectionCoefficient() const;
	T getProjectionCoefficient(const Vector<T, n>& in_point) const;
	Vector<T, n> getProjection(const Vector<T, n>& in_point) const;

	T distanceTo(const Vector<T, n>& in_point) const;
	Vector<T, n> flip(const Vector<T, n>& in_point) const;

public:
	static Line<T, n> fromPoints(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2);
	static Line<T, n> fromPointDirection(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);
};

using Line2f = Line<float, 2>;
using Line2d = Line<double, 2>;
using Line3f = Line<float, 3>;
using Line3d = Line<double, 3>;

#endif


