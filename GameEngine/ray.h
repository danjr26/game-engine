#ifndef RAY_H
#define RAY_H

#include "transform.h"

template<class T, uint n>
class Ray {
private:
	Vector<T, n> point;
	Vector<T, n> direction;

	Ray(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
		point(in_point),
		direction(in_direction)
	{}

public:
	static Ray<T, n> From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
		return Ray<T, n>(in_point, in_direction);
	}

	static Ray<T, n> From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2) {
		return Ray<T, n>(in_point1, in_point2 - in_point1);
	}
};

using Ray2f = Ray<float, 2>;
using Ray2d = Ray<double, 2>;
using Ray3f = Ray<float, 3>;
using Ray3d = Ray<double, 3>;

#endif