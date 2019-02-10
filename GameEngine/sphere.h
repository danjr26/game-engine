#ifndef SPHERE_H
#define SPHERE_H

#include "transform.h"
#include <set>

template<class T, uint n>
class Triangle;

template<class T, uint n = 3>
class Sphere {
private:
	Vector<T, n> mCenter;
	T mRadius;

	Sphere(const Vector<T, n>& in_center, T in_radius);

public:
	Sphere();

	void applyTransform(const Transform<T, n>& transform);

	T getRadius() const;
	void setRadius(T in_radius);
	Vector<T, n> getCenter() const;
	void setCenter(const Vector<T, n>& in_center);
	T getLazyRadius() const;
	Vector<T, n> getExtrema(uint in_dimension, bool in_isPositive) const;

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
	static Sphere<T, n> fromPointRadius(const Vector<T, n>& in_center, T in_radius);
	static Sphere<T, n> fromBoundedPoints(Vector<T, n>* in_points, uint in_nPoints);
	static Sphere<T, n> fromBoundedTriangle(const Triangle<T, n>& in_triangle);

private:
	static Sphere<T, n> welzl(std::set<Vector<T, n>>& in_points, std::set<Vector<T, n>>& in_borderPoints = {});
};

template<class T>
using Circle = Sphere<T, 2>;
using Circlef = Circle<float>;
using Circled = Circle<double>;

#endif


