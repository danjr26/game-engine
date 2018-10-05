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

	Vector<T, n> Get_Direction() const {
		return direction;
	}

	T Get_Projection_Coefficient() const {
		return direction.Dot(point);
	}

	T Get_Projection_Coefficient(const Vector<T, n>& in_point) {
		return (in_point - point).Projection_Coeff(direction);
	}

	Vector<T, n> Get_Projection(const Vector<T, n>& in_point) {
		return (in_point - point).Projection(direction) + point;
	}

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