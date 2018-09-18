#ifndef HALF_SPACE_H
#define HALF_SPACE_H

#include "vector.h"

template<class T, uint n>
class HalfSpace {
private:
	Vector<T, n> point;
	Vector<T, n> direction;

	HalfSpace(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
		point(in_point),
		direction(in_direction)
	{}

public:
	static HalfSpace<T, n> From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
		return HalfSpace<T, n>(in_point, in_direction);
	}
};

using HalfSpace2f = HalfSpace<float, 2>;
using HalfSpace2d = HalfSpace<double, 2>;
using HalfSpace3f = HalfSpace<float, 3>;
using HalfSpace3d = HalfSpace<double, 3>;

#endif
