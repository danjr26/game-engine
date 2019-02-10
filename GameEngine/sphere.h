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

	void Apply_Transform(const Transform<T, n>& transform);

	T Get_Radius() const;
	void Set_Radius(T in_radius);
	Vector<T, n> Get_Center() const;
	void Set_Center(const Vector<T, n>& in_center);
	T Get_Lazy_Radius() const;
	Vector<T, n> Get_Extrema(uint in_dimension, bool in_isPositive) const;

	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Area() const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Circumference() const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Volume() const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Surface_Area() const;
	template<typename = typename std::enable_if_t<n == 3, void>>
	T Get_Major_Circumference() const;

	template<typename = typename std::enable_if_t<n == 2, void>>
	Vector<T, n> Random_Point_Boundary() const;
	template<typename = typename std::enable_if_t<n == 3, void>, typename = void>
	Vector<T, n> Random_Point_Boundary() const;
	Vector<T, n> Random_Point_Inside() const;

public:
	static Sphere<T, n> From_Point_Radius(const Vector<T, n>& in_center, T in_radius);
	static Sphere<T, n> From_Bounded_Points(Vector<T, n>* in_points, uint in_nPoints);
	static Sphere<T, n> From_Bounded_Triangle(const Triangle<T, n>& in_triangle);

private:
	static Sphere<T, n> Welzl(std::set<Vector<T, n>>& in_points, std::set<Vector<T, n>>& in_borderPoints = {});
};

template<class T>
using Circle = Sphere<T, 2>;
using Circlef = Circle<float>;
using Circled = Circle<double>;

#endif


