#ifndef SPHERE_H
#define SPHERE_H

#include "geometric.h"
#include <set>

template<class T, uint n>
class Triangle;

template<class T, uint n = 3>
class Sphere : public Geometric<T, n> {
private:
	Vector<T, n> mCenter;
	T mRadius;

	Sphere(const Vector<T, n>& i_center, T i_radius);

public:
	Sphere();

	void applyTransform(const Transform<T, n>& i_transform) override;
	void getClosest(ClosestRequest<T, n>& io_request) const override;

	T getRadius() const;
	void setRadius(T i_radius);
	Vector<T, n> getCenter() const;
	void setCenter(const Vector<T, n>& i_center);
	T getLazyRadius() const;
	Vector<T, n> getExtrema(uint i_dimension, bool i_isPositive) const;
	Range<T> getRange(uint i_dimension) const;

	template<typename = typename std::enable_if_t<n == 2, void>>
	T getArea() const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	T getCircumference() const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	T getVolume() const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	T getSurfaceArea() const;
	template<typename = typename std::enable_if_t<n == 3, void>>
	T getMajorCircumference() const;

	template<typename = typename std::enable_if_t<n == 2, void>>
	Vector<T, n> randomPointBoundary() const;
	template<typename = typename std::enable_if_t<n == 3, void>, typename = void>
	Vector<T, n> randomPointBoundary() const;
	Vector<T, n> randomPointInside() const;

public:
	static Sphere<T, n> fromPointRadius(const Vector<T, n>& i_center, T i_radius);
	static Sphere<T, n> fromBoundedPoints(Vector<T, n>* i_points, uint i_nPoints);
	static Sphere<T, n> fromBoundedTriangle(const Triangle<T, n>& i_triangle);

private:
	static Sphere<T, n> welzl(std::set<Vector<T, n>>& i_points, std::set<Vector<T, n>>& i_borderPoints = {});
};

using Sphere2f = Sphere<float, 2>;
using Sphere2d = Sphere<double, 2>;
using Sphere3f = Sphere<float, 3>;
using Sphere3d = Sphere<double, 3>;

#endif


