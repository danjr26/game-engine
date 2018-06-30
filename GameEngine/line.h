#ifndef LINE_H
#define LINE_H

#include "vector.h"
#include "line_segment.h"
#include "ray.h"

template<class T, uint n>
class Line {
public:
	Vector<T, n> point;
	Vector<T, n> direction;

	Line(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
		point(in_point),
		direction(in_direction)
	{}

	explicit Line(const Ray<T, n>& in_ray) :
		point(in_ray.point),
		direction(in_ray.direction)
	{}

	explicit Line(const LineSegment<T, n>& in_segment) :
		point(in_segment.point1),
		direction(in_segment.point2 - in_segmenet.point1)
	{}
};

#endif