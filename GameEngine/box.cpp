#include "box.h"

template<class T, uint n>
inline Box<T, n>::Box(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes) :
	origin(in_origin) {

	for (uint i = 0; i < n; i++) axes[i] = in_axes[i];
}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::Get_Origin() {
	return origin;
}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::Get_Center() {
	Vector<T, n> out = origin;
	for (uint i = 0; i < n; i++) out += axes[i] / 2.0;
	return out;
}

template<class T, uint n>
inline void Box<T, n>::Get_Axes(Vector<T, n>* out_axes) {
	for (uint i = 0; i < n; i++) out_axes[i] = axes[i];
}

template<class T, uint n>
inline void Box<T, n>::Get_Corners(Vector<T, n>* out_corners) {
	const uint nCorners = 1 << n;

	for (uint i = 0; i < nCorners; i++) {
		out_corners[i] = origin;
		for (uint j = 0; j < n; j++) {
			if ((i >> j) & 1) out_corners[i] += axes[j];
		}
	}
}

template<class T, uint n>
inline void Box<T, n>::Apply_Transform(Transform<T, n>& transform) {
	origin = transform.Apply_To_Local_Point(origin);
	for (uint i = 0; i < n; i++) {
		axes[i] = transform.Apply_To_Local_Vector(axes[i]);
	}
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::Get_Area() const {
	return axes[0].Magnitude() * axes[1].Magnitude();
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::Get_Perimeter() const {
	return (axes[0].Magnitude() + axes[1].Magnitude()) * 2.0;
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::Get_Volume() const {
	return axes[0].Magnitude() * axes[1].Magnitude() * axes[2].Magnitude();
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::Get_Surface_Area() const {
	Vector<T, n> edgeLengths = {
		axes[0].Magnitude(),
		axes[1].Magnitude(),
		axes[2].Magnitude()
	};
	return (
		edgeLengths.X() * edgeLengths.Y() +
		edgeLengths.Y() * edgeLengths.Z() +
		edgeLengths.Z() * edgeLengths.X()
		) * 2.0;
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::Get_Total_Edge_Length() const {
	return (axes[0].Magnitude() + axes[1].Magnitude() + axes[2].Magnitude()) * 2.0;
}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::Random_Point_Boundary() const {
	Vector<T, n> out;
	uint stuckDimension = Random<uint>(n);
	for (uint i = 0; i < n; i++) {
		if (i == stuckDimension) {
			out += (Random<bool>()) ? Vector<T, n>() : axes[i];
		}
		else {
			out += axes[i] * Random<T>(1);
		}
	}
	return out;
}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::Random_Point_Inside() const {
	Vector<T, n> out;
	for (uint i = 0; i < n; i++) {
		out += axes[i] * Random<T>(1);
	}
	return out;
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::From_Center_Axes(const Vector<T, n>& in_center, const Vector<T, n>* in_axes) {
	Vector<T, n> origin = in_center;
	for (uint i = 0; i < n; i++) origin -= in_axes[i] / 2.0;
	return Box<T, n>(origin, in_axes);
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::From_Center_Axes(const Vector<T, n>& in_center, std::initializer_list<Vector<T, n>> in_axes) {
	Vector<T, n> origin = in_center;
	for (uint i = 0; i < n; i++) origin -= in_axes.begin()[i] / 2.0;
	return Box<T, n>(origin, in_axes.begin());
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::From_Origin_Axes(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes) {
	return Box<T, n>(in_origin, in_axes);
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::From_Origin_Axes(const Vector<T, n>& in_origin, std::initializer_list<Vector<T, n>> in_axes) {
	return Box<T, n>(in_origin, in_axes.begin());
}

template class Box<float, 2>;
template class Box<double, 2>;
template class Box<float, 3>;
template class Box<double, 3>;