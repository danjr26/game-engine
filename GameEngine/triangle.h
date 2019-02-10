#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "transform.h"

template<class T, uint n>
class Triangle {
private:
	Vector<T, n> mPoints[3];

	Triangle(const Vector<T, n>* in_points);

public:
	Triangle();

	void applyTransform(const Transform<T, n>& in_transform);

	void getPoints(Vector<T, n>* out_points) const;
	void getPointOffsets(Vector<T, n>* out_offsets) const;

	T getAngle(uint in_index) const;
	T getSideLength(uint in_index) const;

	template<typename = typename std::enable_if_t<n == 2, void>>
	void getLazyNormals(Vector<T, n>* out_lazyNormals) const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	void getNormals(Vector<T, n>* out_Normals) const;
	template<typename = typename std::enable_if_t<n = 3, void>>
	Vector<T, n> getLazyNormal() const;
	template<typename = typename std::enable_if_t<n = 3, void>>
	Vector<T, n> getNormal() const;

	Vector<T, n>& operator[](uint in_index);
	Vector<T, n> getPoint(uint in_index) const;
	Vector<T, n> getPointOffset(uint in_index) const;

	template<typename = typename std::enable_if_t<n == 2, void>>
	T getArea() const;
	template<typename = typename std::enable_if_t<n == 3, void>, typename = void>
	T getArea() const;
	T getPerimeter() const;

	T getCircumradius() const;
	Vector<T, n> getCircumcenter() const;

	Vector<T, n> randomPointBoundary() const;
	template<typename = typename std::enable_if_t<n == 2, void>>
	Vector<T, n> randomPointInside() const;
	template<typename = typename std::enable_if_t<n == 3, void>, typename = void>
	Vector<T, n> randomPointInside() const;

public:
	static Triangle<T, n> fromPoints(const Vector<T, n>* in_points);
	static Triangle<T, n> fromPoints(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2, const Vector<T, n>& in_point3);
};

using Triangle2f = Triangle<float, 2>;
using Triangle2d = Triangle<double, 2>;
using Triangle3f = Triangle<float, 3>;
using Triangle3d = Triangle<double, 3>;

#endif


