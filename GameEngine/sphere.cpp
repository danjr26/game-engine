#include "sphere.h"
#include "exceptions.h"

template<class T, uint n>
inline Sphere<T, n>::Sphere(const Vector<T, n>& in_center, T in_radius) :
	center(in_center),
	radius(in_radius) {}

template<class T, uint n>
inline void Sphere<T, n>::Apply_Transform(Transform<T, n>& transform) {
	T scale = 1.0;
	for (Transform<T, n>* t = &transform; t != nullptr; t = t->Get_Parent()) {
		scale *= t->Get_Local_Scale().Max_Component();
	}
	(*this) = Sphere<T, n>::From_Point_Radius(
		transform.Apply_To_Local_Point(center),
		radius * scale
	);
}

template<class T, uint n>
inline T Sphere<T, n>::Get_Radius() const {
	return radius;
}

template<class T, uint n>
inline void Sphere<T, n>::Set_Radius(T in_radius) {
	radius = in_radius;
}

template<class T, uint n>
inline Vector<T, n> Sphere<T, n>::Get_Center() const {
	return center;
}

template<class T, uint n>
inline void Sphere<T, n>::Set_Center(const Vector<T, n>& in_center) {
	center = in_center;
}

template<class T, uint n>
inline Vector<T, n> Sphere<T, n>::Get_Extrema(uint in_dimension, bool in_isPositive) const {
	if (in_dimension >= n) {
		throw InvalidArgumentException();
	}
	Vector<T, n> out = center;
	out[in_dimension] += (in_isPositive) ? radius : -radius;
	return out;
}

template<class T, uint n>
template<typename>
Vector<T, n> Sphere<T, n>::Random_Point_Boundary() const {
	T angle = Random<T>((T)(2 * PI));
	return Vector<T, n>(cos(angle), sin(angle)) * radius;
}

template<class T, uint n>
template<typename, typename>
Vector<T, n> Sphere<T, n>::Random_Point_Boundary() const {
	T angle = Random<T>((T)(2 * PI));
	T z = Random<T>(-1, 1);
	T u = sqrt(1 - z * z);
	return center + Vector<T, n>(u * cos(angle), u * sin(angle), z) * radius;
}

template<class T, uint n>
Vector<T, n> Sphere<T, n>::Random_Point_Inside() const {
	return (Random_Point_Boundary() - center) * sqrt(Random<T>(1)) + center;
}

template<class T, uint n>
inline Sphere<T, n> Sphere<T, n>::From_Point_Radius(const Vector<T, n>& in_center, T in_radius) {
	return Sphere(in_center, in_radius);
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Area() const {
	return PI * radius * radius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Circumference() const {
	return 2.0 * PI * radius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Volume() const {
	return 4.0 / 3.0 * PI * radius * radius * radius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Surface_Area() const {
	return 4.0 * PI * radius * radius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Major_Circumference() const {
	return 2.0 * PI * radius;
}

template class Sphere<float, 2>;
template class Sphere<double, 2>;
template class Sphere<float, 3>;
template class Sphere<double, 3>;