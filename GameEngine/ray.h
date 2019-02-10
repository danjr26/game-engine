#ifndef RAY_H
#define RAY_H

#include "transform.h"

template<class T, uint n>
class Ray {
private:
	Vector<T, n> mPoint;
	Vector<T, n> mDirection;

	Ray(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);

public:
	void Apply_Transform(const Transform<T, n>& in_transform);

	Vector<T, n> Get_Point() const;
	Vector<T, n> Get_Direction() const;
	T Get_Projection_Coefficient() const;
	T Get_Projection_Coefficient(const Vector<T, n>& in_point) const;
	Vector<T, n> Get_Projection(const Vector<T, n>& in_point) const;

public:
	static Ray<T, n> From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);
	static Ray<T, n> From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2);
};

using Ray2f = Ray<float, 2>;
using Ray2d = Ray<double, 2>;
using Ray3f = Ray<float, 3>;
using Ray3d = Ray<double, 3>;

#endif


