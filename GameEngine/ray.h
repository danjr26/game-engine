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
	Ray();

	void applyTransform(const Transform<T, n>& in_transform);

	Vector<T, n> getPoint() const;
	Vector<T, n> getDirection() const;
	T getProjectionCoefficient() const;
	T getProjectionCoefficient(const Vector<T, n>& in_point) const;
	Vector<T, n> getProjection(const Vector<T, n>& in_point) const;

public:
	static Ray<T, n> fromPointDirection(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);
	static Ray<T, n> fromPoints(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2);
};

using Ray2f = Ray<float, 2>;
using Ray2d = Ray<double, 2>;
using Ray3f = Ray<float, 3>;
using Ray3d = Ray<double, 3>;

#endif


