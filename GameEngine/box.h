#ifndef BOX_H
#define BOX_H

#include "transformable_object.h"
#include "matrix.h"
#include "definitions.h"
#include <initializer_list>

template<class T, uint n>
class Box {
private:
	Vector<T, n> mOrigin;
	Vector<T, n> mAxes[n];

private:
	Box(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes);

public:
	Box();

	Vector<T, n> getOrigin() const;
	Vector<T, n> getCenter() const;
	void getAxes(Vector<T, n>* out_axes) const;
	void getCorners(Vector<T, n>* out_corners) const;
	void applyTransform(const Transform<T, n>& transform);

	template<typename = typename std::enable_if_t<n == 2>>
	T getArea() const;
	template<typename = typename std::enable_if_t<n == 2>>
	T getPerimeter() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T getVolume() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T getSurfaceArea() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T getTotalEdgeLength() const;

	Vector<T, n> randomPointBoundary() const;
	Vector<T, n> randomPointInside() const;

public:
	static Box<T, n> fromCenterAxes(const Vector<T, n>& in_center, const Vector<T, n>* in_axes);
	static Box<T, n> fromCenterAxes(const Vector<T, n>& in_center, std::initializer_list<Vector<T, n>> in_axes);
	static Box<T, n> fromOriginAxes(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes);
	static Box<T, n> fromOriginAxes(const Vector<T, n>& in_origin, std::initializer_list<Vector<T, n>> in_axes);
};

using Rectanglef = Box<float, 2>;
using Rectangled = Box<double, 2>;
using Boxf = Box<float, 3>;
using Boxd = Box<double, 3>;

#endif



