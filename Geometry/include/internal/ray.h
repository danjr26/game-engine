#ifndef RAY_H
#define RAY_H

#include "geometric.h"

template<class T, uint>
class Line;

template<class T, uint n>
class Ray : public Geometric<T, n> {
private:
	Vector<T, n> mPoint;
	Vector<T, n> mDirection;

	Ray(const Vector<T, n>& i_point, const Vector<T, n>& i_direction);

public:
	Ray();

	Line<T, n> toLine() const;

	void applyTransform(const Transform<T, n>& i_transform) override;
	void getClosest(ClosestRequest<T, n>& io_request) const override;

	Vector<T, n> getPoint() const;
	Vector<T, n> getDirection() const;
	T getProjectionCoefficient() const;
	T getProjectionCoefficient(const Vector<T, n>& i_point) const;
	Vector<T, n> getProjection(const Vector<T, n>& i_point) const;

public:
	static Ray<T, n> fromPointDirection(const Vector<T, n>& i_point, const Vector<T, n>& i_direction);
	static Ray<T, n> fromPoints(const Vector<T, n>& i_point1, const Vector<T, n>& i_point2);
};

using Ray2f = Ray<float, 2>;
using Ray2d = Ray<double, 2>;
using Ray3f = Ray<float, 3>;
using Ray3d = Ray<double, 3>;

#endif


