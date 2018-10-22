#ifndef BOX_H
#define BOX_H

#include "transformable_object.h"
#include "matrix.h"
#include "definitions.h"
#include <initializer_list>

template<class T, uint n>
struct Box {
private:
	Vector<T, n> origin;
	Vector<T, n> axes[n];

private:
	Box(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes) :
		origin(in_origin) {

		for (uint i = 0; i < n; i++) axes[i] = in_axes[i];
	}

public:
	Vector<T, n> Get_Origin() {
		return origin;
	}

	Vector<T, n> Get_Center() {
		Vector2d out = origin;
		for (uint i = 0; i < n; i++) out += axes[i] / 2.0;
		return out;
	}

	void Get_Axes(Vector<T, n>* out_axes) {
		for (uint i = 0; i < n; i++) out_axes[i] = axes[i];
	}

	void Get_Corners(Vector<T, n>* out_corners) {
		const uint nCorners = 1 << n;

		for (uint i = 0; i < nCorners; i++) {
			out_corners[i] = origin;
			for (uint j = 0; j < n; j++) {
				if ((i >> j) & 1) out_corners[i] += axes[j];
			}
		}
	}

	void Apply_Transform(Transform& transform) {
		origin = Vector<T, n>(transform.Apply_To_Local_Point(Vector<T, 3>(origin)));
		for (uint i = 0; i < n; i++) {
			axes[i] = Vector<T, n>(transform.Apply_To_Local_Vector(Vector<T, 3>(axes[i])));
		}
	}

	template<typename = typename std::enable_if_t<n == 2>>
	T Get_Area() const {
		return axes[0].Magnitude() * axes[1].Magnitude();
	}

	template<typename = typename std::enable_if_t<n == 2>>
	T Get_Perimeter() const {
		return (axes[0].Magnitude() + axes[1].Magnitude()) * 2.0;
	}

	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Volume() const {
		return axes[0].Magnitude() * axes[1].Magnitude() * axes[2].Magnitude();
	}

	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Surface_Area() const {
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

	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Total_Edge_Length() const {
		return (axes[0].Magnitude() + axes[1].Magnitude() + axes[2].Magnitude()) * 2.0;
	}

	static Box From_Center_Axes(const Vector<T, n>& in_center, const Vector<T, n>* in_axes) {
		Vector2d origin = in_center;
		for (uint i = 0; i < n; i++) origin -= in_axes[i] / 2.0;
		return Box(origin, in_axes);
	}

	static Box From_Center_Axes(const Vector<T, n>& in_origin, std::initializer_list<Vector<T, n>> in_axes) {
		Vector2d origin = in_center;
		for (uint i = 0; i < n; i++) origin -= in_axes.begin()[i] / 2.0;
		return Box(origin, in_axes.begin());
	}

	static Box From_Origin_Axes(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes) {
		return Box(in_origin, in_axes);
	}

	static Box From_Origin_Axes(const Vector<T, n>& in_origin, std::initializer_list<Vector<T, n>> in_axes) {
		return Box(in_origin, in_axes.begin());
	}
};

using Rectanglef = Box<float, 2>;
using Rectangled = Box<double, 2>;
using Boxf = Box<float, 3>;
using Boxd = Box<double, 3>;

#endif