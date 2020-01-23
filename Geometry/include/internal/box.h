#ifndef BOX_H
#define BOX_H

#include "geometric.h"

template<class T, uint n>
class Box : public Geometric<T, n> {
private:
	Vector<T, n> mOrigin;
	Vector<T, n> mAxes[n];

private:
	Box(const Vector<T, n>& i_origin, const Vector<T, n>* i_axes);

public:
	Box();

	Vector<T, n> getOrigin() const;
	Vector<T, n> getCenter() const;
	void getAxes(Vector<T, n>* o_axes) const;
	void getNormalAxes(Vector<T, n>* o_axes) const;
	void getCorners(Vector<T, n>* o_corners) const;

	void applyTransform(const Transform<T, n>& i_transform) override;
	void getClosest(ClosestRequest<T, n>& io_request) const override;

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
	static Box<T, n> fromCenterAxes(const Vector<T, n>& i_center, const Vector<T, n>* i_axes);
	static Box<T, n> fromCenterAxes(const Vector<T, n>& i_center, std::initializer_list<Vector<T, n>> i_axes);
	static Box<T, n> fromOriginAxes(const Vector<T, n>& i_origin, const Vector<T, n>* i_axes);
	static Box<T, n> fromOriginAxes(const Vector<T, n>& i_origin, std::initializer_list<Vector<T, n>> i_axes);
};

using Rectanglef = Box<float, 2>;
using Rectangled = Box<double, 2>;
using Boxf = Box<float, 3>;
using Boxd = Box<double, 3>;

#endif



