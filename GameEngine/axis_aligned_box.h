#ifndef AXIS_ALIGNED_BOX_H
#define AXIS_ALIGNED_BOX_H

#include "transform.h"
#include "range.h"

template<class T, uint n = 3>
class AxisAlignedBox {
private:
	Vector<T, n> mMinima;
	Vector<T, n> mMaxima;

private:
	AxisAlignedBox(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima);

public:
	void applyTransform(const Transform<T, n>& transform);

	void getCorners(Vector<T, n>* out_corners) const;
	Vector<T, n> getCenter() const;
	Vector<T, n> getDimensions() const;
	Vector<T, n> getMinima() const;
	void setMinima(const Vector<T, n>& in_minima);
	Vector<T, n> getMaxima() const;
	void setMaxima(const Vector<T, n>& in_maxima);
	Range<T> getRange(uint dimension) const;

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
	static AxisAlignedBox<T, n> fromExtrema(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima);
	static AxisAlignedBox<T, n> fromCenter(const Vector<T, n>& in_center, const Vector<T, n>& in_dimensions);
	static AxisAlignedBox<T, n> fromBoundedPoints(uint in_nPoints, const Vector<T, n>* in_points);
};

template<class T>
using AxisAlignedRectangle = AxisAlignedBox<T, 2>;
using AxisAlignedRectanglef = AxisAlignedRectangle<float>;
using AxisAlignedRectangled = AxisAlignedRectangle<double>;
using AxisAlignedBoxf = AxisAlignedBox<float, 3>;
using AxisAlignedBoxd = AxisAlignedBox<double, 3>;

#endif

