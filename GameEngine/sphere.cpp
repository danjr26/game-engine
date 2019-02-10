#include "sphere.h"
#include "exceptions.h"
#include "triangle.h"
#include <set>

template<class T, uint n>
inline Sphere<T, n>::Sphere(const Vector<T, n>& in_center, T in_radius) :
	mCenter(in_center),
	mRadius(in_radius) {}

template<class T, uint n>
Sphere<T, n>::Sphere() :
	mCenter(),
	mRadius(0)
{}

template<class T, uint n>
inline void Sphere<T, n>::Apply_Transform(const Transform<T, n>& transform) {
	T scale = 1.0;
	for (Transform<T, n>const* t = &transform; t != nullptr; t = t->Get_Const_Parent()) {
		scale *= t->Get_Local_Scale().Max_Component();
	}
	(*this) = Sphere<T, n>::From_Point_Radius(
		transform.Local_To_World_Point(mCenter),
		mRadius * scale
	);
}

template<class T, uint n>
inline T Sphere<T, n>::Get_Radius() const {
	return mRadius;
}

template<class T, uint n>
inline void Sphere<T, n>::Set_Radius(T in_radius) {
	mRadius = in_radius;
}

template<class T, uint n>
inline Vector<T, n> Sphere<T, n>::Get_Center() const {
	return mCenter;
}

template<class T, uint n>
inline void Sphere<T, n>::Set_Center(const Vector<T, n>& in_center) {
	mCenter = in_center;
}

template<class T, uint n>
T Sphere<T, n>::Get_Lazy_Radius() const {
	return mRadius * mRadius;
}

template<class T, uint n>
inline Vector<T, n> Sphere<T, n>::Get_Extrema(uint in_dimension, bool in_isPositive) const {
	if (in_dimension >= n) {
		throw InvalidArgumentException();
	}
	Vector<T, n> out = mCenter;
	out[in_dimension] += (in_isPositive) ? mRadius : -mRadius;
	return out;
}

template<class T, uint n>
template<typename>
Vector<T, n> Sphere<T, n>::Random_Point_Boundary() const {
	T angle = Random<T>((T)(2 * PI));
	return Vector<T, n>(cos(angle), sin(angle)) * mRadius;
}

template<class T, uint n>
template<typename, typename>
Vector<T, n> Sphere<T, n>::Random_Point_Boundary() const {
	T angle = Random<T>((T)(2 * PI));
	T z = Random<T>(-1, 1);
	T u = sqrt(1 - z * z);
	return mCenter + Vector<T, n>(u * cos(angle), u * sin(angle), z) * mRadius;
}

template<class T, uint n>
Sphere<T, n> Sphere<T, n>::From_Bounded_Points(Vector<T, n>* in_points, uint in_nPoints) {
	std::set<Vector<T, n>> points(in_points, in_points + in_nPoints);
	std::set<Vector<T, n>> boundaryPoints;
	return Welzl(points, boundaryPoints);
}

template<class T, uint n>
Sphere<T, n> Sphere<T, n>::From_Bounded_Triangle(const Triangle<T, n>& in_triangle) {
	for (uint i = 0; i < 3; i++) {
		if (in_triangle.Get_Angle(i) >= PI / 2) {
			Vector<T, n> center = (in_triangle.Get_Point((i + 1) % 3) + in_triangle.Get_Point((i + 2) % 3)) * (T)0.5;
			T radius = (in_triangle.Get_Point((i + 1) % 3) - center).Magnitude();
			return Sphere<T, n>(center, radius);
		}
	}

	return Sphere<T, n>(in_triangle.Get_Circumcenter(), in_triangle.Get_Circumradius());
}

template<class T, uint n>
Sphere<T, n> Sphere<T, n>::Welzl(std::set<Vector<T, n>>& in_points, std::set<Vector<T, n>>& in_borderPoints) {
	if (in_points.empty() || in_borderPoints.size() >= 3) {
		Vector<T, n> radius;
		auto it = in_borderPoints.begin();
		switch (in_borderPoints.size()) {
		case 0:
			return { Vector<T, n>(), (T)0 };
		case 1:
			return { *in_borderPoints.begin(), (T)0 };
		case 2:
			radius = (*in_borderPoints.end() - *in_borderPoints.begin()) * 0.5;
			return { *in_borderPoints.begin() + radius, radius.Magnitude() };
		default:
			return Sphere<T, n>::From_Bounded_Triangle(Triangle<T, n>::From_Points(*(it++), *(it++), *(it++)));
		}
	}
	Vector<T, n> point = *in_points.begin();
	in_points.erase(in_points.begin());
	Sphere<T, n> lastSphere = Welzl(in_points, in_borderPoints);
	if ((point - lastSphere.Get_Center()).Dot_Self() <= lastSphere.Get_Lazy_Radius()) {
		return lastSphere;
	}
	in_borderPoints.insert(point);
	Sphere<T, n> newSphere = Welzl(in_points, in_borderPoints);
	in_borderPoints.erase(in_borderPoints.find(point));
	in_points.insert(point);
	return newSphere;
}

template<class T, uint n>
Vector<T, n> Sphere<T, n>::Random_Point_Inside() const {
	return (Random_Point_Boundary() - mCenter) * sqrt(Random<T>(1)) + mCenter;
}

template<class T, uint n>
inline Sphere<T, n> Sphere<T, n>::From_Point_Radius(const Vector<T, n>& in_center, T in_radius) {
	return Sphere(in_center, in_radius);
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Area() const {
	return PI * mRadius * mRadius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Circumference() const {
	return 2.0 * PI * mRadius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Volume() const {
	return 4.0 / 3.0 * PI * mRadius * mRadius * mRadius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Surface_Area() const {
	return 4.0 * PI * mRadius * mRadius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::Get_Major_Circumference() const {
	return 2.0 * PI * mRadius;
}

template class Sphere<float, 2>;
template class Sphere<double, 2>;
template class Sphere<float, 3>;
template class Sphere<double, 3>;