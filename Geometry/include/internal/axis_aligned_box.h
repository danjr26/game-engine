#ifndef AXIS_ALIGNED_BOX_H
#define AXIS_ALIGNED_BOX_H

#include "../../../Utilities/include/utilities.h"
#include "geometric.h"

template<class T, uint n>
class Sphere;

template<class T, uint n>
class AABox : public Geometric<T, n> {
private:
	Vector<T, n> mMinima;
	Vector<T, n> mMaxima;

private:
	AABox(const Vector<T, n>& i_minima, const Vector<T, n>& i_maxima);

public:
	AABox();

	void applyTransform(const Transform<T, n>& i_transform) override;
	void getClosest(ClosestRequest<T, n>& io_request) const override;

	void getCorners(Vector<T, n>* o_corners) const;
	Vector<T, n> getCenter() const;
	Vector<T, n> getDimensions() const;
	Vector<T, n> getMinima() const;
	void setMinima(const Vector<T, n>& i_minima);
	Vector<T, n> getMaxima() const;
	void setMaxima(const Vector<T, n>& i_maxima);
	Range<T> getRange(uint dimension) const;
	bool containsPoint(const Vector<T, n>& i_point) const;

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
	static AABox<T, n> fromExtrema(const Vector<T, n>& i_minima, const Vector<T, n>& i_maxima);
	static AABox<T, n> fromRanges(const Range<T>* i_ranges);
	static AABox<T, n> fromCenter(const Vector<T, n>& i_center, const Vector<T, n>& i_dimensions);
	static AABox<T, n> fromBoundedPoints(uint i_nPoints, const Vector<T, n>* i_points);
	static AABox<T, n> fromBoundedSphere(const Sphere<T, n>& i_sphere);
};

using AABox2f = AABox<float, 2>;
using AABox2d = AABox<double, 2>;
using AABox3f = AABox<float, 3>;
using AABox3d = AABox<double, 3>;

#endif

