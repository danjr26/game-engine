#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "transform.h"

template<class T, uint n>
class Triangle {
private:
	Vector<T, n> points[3];

	Triangle(const Vector<T, n>* in_points);

public:
	Triangle();

	void Apply_Transform(const Transform<T, n>& in_transform);

	void Get_Points(Vector<T, n>* out_points) const;
	void Get_Point_Offsets(Vector<T, n>* out_offsets) const;

	T Get_Angle(uint in_index) const;
	T Get_Side_Length(uint in_index) const;

	template<typename = typename std::enable_if_t<n == 2, void>>
	void Get_Lazy_Normals(Vector<T, n>* out_lazyNormals) const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	void Get_Normals(Vector<T, n>* out_Normals) const;
	template<typename = typename std::enable_if_t<n = 3, void>>
	Vector<T, n> Get_Lazy_Normal() const;
	template<typename = typename std::enable_if_t<n = 3, void>>
	Vector<T, n> Get_Normal() const;

	Vector<T, n>& operator[](uint in_index);
	Vector<T, n> Get_Point(uint in_index) const;
	Vector<T, n> Get_Point_Offset(uint in_index) const;

	template<typename = typename std::enable_if_t<n == 2, void>>
	T Get_Area() const;
	template<typename = typename std::enable_if_t<n == 3, void>, typename = void>
	T Get_Area() const;
	T Get_Perimeter() const;

	T Get_Circumradius() const;
	Vector<T, n> Get_Circumcenter() const;

	Vector<T, n> Random_Point_Boundary() const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	Vector<T, n> Random_Point_Inside() const;
	template<typename = typename std::enable_if_t<n == 3, void>, typename = void>
	Vector<T, n> Random_Point_Inside() const;

public:
	static Triangle<T, n> From_Points(const Vector<T, n>* in_points);
	static Triangle<T, n> From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2, const Vector<T, n>& in_point3);
};

using Triangle2f = Triangle<float, 2>;
using Triangle2d = Triangle<double, 2>;
using Triangle3f = Triangle<float, 3>;
using Triangle3d = Triangle<double, 3>;

#endif


