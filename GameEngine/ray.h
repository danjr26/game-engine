#ifndef RAY_H
#define RAY_H

#include "vector.h"

template<class T, uint n>
struct Ray {
	Vector<T, n> point;
	Vector<T, n> direction;

	Ray(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
		point(in_point),
		direction(in_direction)
	{}
};

#endif