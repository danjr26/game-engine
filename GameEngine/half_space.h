#ifndef HALF_SPACE_H
#define HALF_SPACE_H

#include "transform.h"

template<class T, uint n>
class HalfSpace {
private:
	Vector<T, n> point;
	Vector<T, n> direction;

	HalfSpace(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);

public:
	void Apply_Transform(const Transform<T, n>& in_transform);
	Vector<T, n> Get_Point() const;
	void Set_Point(const Vector<T, n>& in_point);
	Vector<T, n> Get_Direction() const;
	void Set_Direction(const Vector<T, n>& in_direction);
	T Get_Projection_Coefficient() const;
	Vector<T, n> Get_Projection() const;

public:
	static HalfSpace<T, n> From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);
};

using HalfSpace2f = HalfSpace<float, 2>;
using HalfSpace2d = HalfSpace<double, 2>;
using HalfSpace3f = HalfSpace<float, 3>;
using HalfSpace3d = HalfSpace<double, 3>;

#endif


