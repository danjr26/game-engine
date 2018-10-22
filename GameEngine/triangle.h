#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "transform.h"

template<class T, uint n>
class Triangle {
private:
	Vector<T, n> points[3];

	Triangle(const Vector<T, n>* in_points) {
		for (uint i = 0; i < 3; i++) points[i] = in_points[i];
	}

public:
	void Apply_Transform(Transform& in_transform) {
		for (uint i = 0; i < 3; i++) {
			points[i] = Vector<T, n>(in_transform.Apply_To_Local_Point(Vector<T, 3>(points[i])));
		}
	}

	void Get_Points(Vector<T, n>* out_points) {
		for (uint i = 0; i < 3; i++) out_points[i] = points[i];
	}

	void Get_Point_Offsets(Vector<T, n>* out_offsets) {
		for (uint i = 0; i < 3; i++) out_offsets[i] = points[(i + 1) % 3] - points[i];
	}

	template<typename = typename std::enable_if_t<n == 2, void>>
	void Get_Lazy_Normals(Vector<T, n>* out_lazyNormals) {
		for (uint i = 0; i < 3; i++) out_lazyNormals[i] = (points[i] - points[(i + 1) % 3]).Orthogonal();
	}

	template<typename = typename std::enable_if_t<n == 2, void>>
	void Get_Normals(Vector<T, n>* out_Normals) {
		for (uint i = 0; i < 3; i++) out_Normals[i] = (points[i] - points[(i + 1) % 3]).Orthogonal().Normalized();
	}

	Vector<T, n>& operator[](uint in_index) {
		return points[in_index];
	}

	Vector<T, n> Get_Point(uint in_index) const {
		return points[in_index];
	}

	template<typename = typename std::enable_if_t<n == 2 || n == 3, void>>
	T Get_Area() const {
		if (n == 2) {
			T edgeDots[3] = {
				(points[1] - points[0]).Dot_Self(),
				(points[2] - points[1]).Dot_Self(),
				(points[0] - points[2]).Dot_Self(),
			};

			T term1 = edgeDots[0] + edgeDots[1] + edgeDots[2];
			T term2 = edgeDots[0] * edgeDots[0] + edgeDots[1] * edgeDots[1] + edgeDots[2] * edgeDots[2];

			return 0.25 * sqrt(term1 * term1 - 2 * term2);
		}

		if(n == 3) {
			return 0.5 * (points[1] - points[0]).Cross(points[2] - points[0]).Magnitude();
		}

		return 0.0;
	}

	T Get_Perimeter() const {
		T out = 0;
		for (uint i = 0; i < 3; i++) out += (points[(i + 1) % 3] - points[i]).Magnitude();
		return out;
	}

	static Triangle<T, n> From_Points(const Vector<T, n>* in_points) {
		return Triangle<T, n>(in_points);
	}

	static Triangle<T, n> From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2, const Vector<T, n>& in_point3) {
		Vector<T, n> points[3] = { in_point1, in_point2, in_point3 };
		return Triangle<T, n>(points);
	}
};

using Triangle2f = Triangle<float, 2>;
using Triangle2d = Triangle<double, 2>;
using Triangle3f = Triangle<float, 3>;
using Triangle3d = Triangle<double, 3>;

#endif