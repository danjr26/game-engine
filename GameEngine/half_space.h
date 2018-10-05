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
		direction(in_direction.Normalized())
	{}

public:
	void Apply_Transform(Transform& in_transform) {
		point = Vector<T, n>(in_transform.Apply_To_Local_Point(Vector<T, 3>(point)));
		direction = Vector<T, n>(in_transform.Apply_To_Local_Direction(Vector<T, 3>(direction)));
	}

	Vector<T, n> Get_Point() const {
		return point;
	}

	void Set_Point(const Vector<T, n>& in_point) {
		point = in_point;
	}

	Vector<T, n> Get_Direction() const {
		return direction;
	}

	void Set_Direction(const Vector<T, n>& in_direction) {
		direction = in_direction.Normalized();
	}

	T Get_Projection_Coefficient() const {
		return point.Dot(direction);
	}

	Vector<T, n> Get_Projection() const {
		return point.Dot(direction) * direction;
	}

	static HalfSpace<T, n> From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
		return HalfSpace<T, n>(in_point, in_direction);
	}
};

using HalfSpace2f = HalfSpace<float, 2>;
using HalfSpace2d = HalfSpace<double, 2>;
using HalfSpace3f = HalfSpace<float, 3>;
using HalfSpace3d = HalfSpace<double, 3>;

#endif
