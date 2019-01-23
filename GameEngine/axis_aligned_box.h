#ifndef AXIS_ALIGNED_BOX_H
#define AXIS_ALIGNED_BOX_H

#include "transform.h"
#include "range.h"

template<class T, uint n = 3>
class AxisAlignedBox {
private:
	Vector<T, n> minima;
	Vector<T, n> maxima;

private:
	AxisAlignedBox(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima);

public:
	void Apply_Transform(const Transform<T, n>& transform);

	void Get_Corners(Vector<T, n>* out_corners) const;
	Vector<T, n> Get_Center() const;
	Vector<T, n> Get_Dimensions() const;
	Vector<T, n> Get_Minima() const;
	void Set_Minima(const Vector<T, n>& in_minima);
	Vector<T, n> Get_Maxima() const;
	void Set_Maxima(const Vector<T, n>& in_maxima);
	Range<T> Get_Range(uint dimension) const;

	template<typename = typename std::enable_if_t<n == 2>>
	T Get_Area() const;
	template<typename = typename std::enable_if_t<n == 2>>
	T Get_Perimeter() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Volume() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Surface_Area() const;
	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Total_Edge_Length() const;

	Vector<T, n> Random_Point_Boundary() const;
	Vector<T, n> Random_Point_Inside() const;

public:
	static AxisAlignedBox<T, n> From_Extrema(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima);
	static AxisAlignedBox<T, n> From_Center(const Vector<T, n>& in_center, const Vector<T, n>& in_dimensions);
	static AxisAlignedBox<T, n> From_Bounded_Points(uint in_nPoints, const Vector<T, n>* in_points);
};

template<class T>
using AxisAlignedRectangle = AxisAlignedBox<T, 2>;
using AxisAlignedRectanglef = AxisAlignedRectangle<float>;
using AxisAlignedRectangled = AxisAlignedRectangle<double>;
using AxisAlignedBoxf = AxisAlignedBox<float, 3>;
using AxisAlignedBoxd = AxisAlignedBox<double, 3>;

#endif

