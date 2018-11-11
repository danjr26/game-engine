#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h"

template<class T, uint n = 3>
struct Sphere {
private:
	Vector<T, n> center;
	T radius;

	Sphere(const Vector<T, n>& in_center, T in_radius) :
		center(in_center),
		radius(in_radius)
	{}

public:
	void Apply_Transform(Transform<T, n>& transform) {
		double scale = 1.0;
		for (Transform<T, n>* t = &transform; t != nullptr; t = t->Get_Parent()) {
			scale *= t->Get_Local_Scale().Max_Component();
		}
		(*this) = Sphere<T, n>::From_Point_Radius(
			transform.Apply_To_Local_Point(center),
			radius * scale
		);
	}

	T Get_Radius() const {
		return radius;
	}

	void Set_Radius(T in_radius) {
		radius = in_radius;
	}

	Vector<T, n> Get_Center() const {
		return center;
	}

	void Set_Center(const Vector<T, n>& in_center) {
		center = in_center;
	}

	Vector<T, n> Get_Extrema(uint in_dimension, bool in_isPositive) const {
		if (in_dimension >= n) {
			throw InvalidArgumentException();
		}
		Vector<T, n> out = center;
		out[in_dimension] += (in_isPositive) ? radius : -radius;
		return out;
	}

	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Area() const {
		return PI * radius * radius;
	}

	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Circumference() const {
		return 2.0 * PI * radius;
	}

	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Volume() const {
		return 4.0 / 3.0 * PI * radius * radius * radius;
	}

	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Surface_Area() const {
		return 4.0 * PI * radius * radius;
	}

	template<typename = typename std::enable_if_t<n == 3, void>>
	T Get_Major_Circumference() const {
		return 2.0 * PI * radius;
	}

	static Sphere<T, n> From_Point_Radius(const Vector<T, n>& in_center, T in_radius) {
		return Sphere(in_center, in_radius);
	}
};

template<class T>
using Circle = Sphere<T, 2>;
using Circlef = Circle<float>;
using Circled = Circle<double>;

//template<class T>
//using Sphere = Sphere<T, 3>;

#endif