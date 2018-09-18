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
	void Apply_Transform(Transform& transform) {
		double scale = 1.0;
		for (Transform* t = &transform; t != nullptr; t = t->Get_Parent()) {
			scale *= max(t->Get_Local_Scale().X(), t->Get_Local_Scale().Y());
		}
		(*this) = Sphere<T, n>::From_Point_Radius(
			Vector<T, n>(transform.Apply_To_Local_Point(Vector<T, 3>(center))),
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

	void Set_Center(const Vector<T, n> in_center) {
		center = in_center;
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