#ifndef LINE_H
#define LINE_H

#include "vector.h"
#include "line_segment.h"
#include "ray.h"

template<class T, uint n>
class Line {
private:
	Vector<T, n> point;
	Vector<T, n> direction;

	Line(const Vector<T, n>& in_point, const Vector<T, n> in_direction) :
		point(in_point),
		direction(in_direction)
	{}

public:
	static Line<T, n> From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2) {
		return Line<T, n>(in_point1, in_point2 - in_point1);
	}

	static Line<T, n> From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
		return Line<T, n>(in_point, in_direction);
	}
};

using Line2f = Line<float, 2>;
using Line2d = Line<double, 2>;
using Line3f = Line<float, 3>;
using Line3d = Line<double, 3>;

#endif