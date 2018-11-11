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
	AxisAlignedBox(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima) :
		minima(in_minima),
		maxima(in_maxima) 
	{}

public:
	void Get_Corners(Vector<T, n>* out_corners) {
		const uint nCorners = 1 << n;

		for (uint i = 0; i < nCorners; i++) {
			for (uint j = 0; j < n; j++) {
				out_corners[i][j] = ((i >> j) & 1) ? maxima[j] : minima[j];
			}
		}
	}

	void Apply_Transform(Transform<T, n>& transform) {
		if (transform.Get_World_Rotation().Is_Identity()) {
			for (Transform<T, n>* t = &transform; t != nullptr; t = t->Get_Parent()) {
				minima = minima.Compwise(t->Get_Local_Scale());
				minima += t->Get_Local_Position();

				maxima = maxima.Compwise(t->Get_Local_Scale());
				maxima += t->Get_Local_Position();
			}
			return;
		}

		const uint nCorners = 1 << n;

		Vector<T, n> corners[nCorners];
		Get_Corners(corners);

		for (uint i = 0; i < nCorners; i++) {
			corners[i] = transform.Apply_To_Local_Point(corners[i]);
		}

		(*this) = AxisAlignedBox<T, n>::From_Bounded_Points(nCorners, corners);
	}

	Vector<T, n> Get_Center() const {
		return (minima + maxima) / 2;
	}

	Vector<T, n> Get_Dimensions() const {
		return maxima - minima;
	}

	Vector<T, n> Get_Minima() const {
		return minima;
	}

	void Set_Minima(const Vector<T, n>& in_minima) {
		minima = in_minima;
	}

	Vector<T, n> Get_Maxima() const {
		return maxima;
	}

	void Set_Maxima(const Vector<T, n>& in_maxima) {
		maxima = in_maxima;
	}

	Range<T> Get_Range(uint dimension) {
		if (dimension >= n) {
			throw InvalidArgumentException();
		}
		return Ranged(minima[dimension], maxima[dimension]);
	}

	template<typename = typename std::enable_if_t<n == 2>>
	T Get_Area() const {
		return (maxima - minima).Component_Product();
	}

	template<typename = typename std::enable_if_t<n == 2>>
	T Get_Perimeter() const {
		return (maxima - minima).Component_Sum() * 2.0;
	}

	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Volume() const{
		return (maxima - minima).Component_Product();

	}

	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Surface_Area() const {
		Vector<T, n> edgeLengths = maxima - minima;
		return (
			edgeLengths.X() * edgeLengths.Y() +
			edgeLengths.Y() * edgeLengths.Z() +
			edgeLengths.Z() * edgeLengths.X()
			) * 2.0;
	}

	template<typename = typename std::enable_if_t<n == 3>>
	T Get_Total_Edge_Length() const {
		return (maxima - minima).Component_Sum() * 2.0;
	}

	inline static AxisAlignedBox<T, n> From_Extrema(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima) {
		return AxisAlignedBox(in_minima, in_maxima);
	}

	inline static AxisAlignedBox<T, n> From_Center(const Vector<T, n>& in_center, const Vector<T, n>& in_dimensions) {
		const Vector<T, n> halfDimensions(in_dimensions / 2.0);
		return AxisAlignedBox(in_center - halfDimensions, in_center + halfDimensions);
	}

	inline static AxisAlignedBox<T, n> From_Bounded_Points(uint in_nPoints, const Vector<T, n>* in_points) {
		if (in_nPoints == 0) return AxisAlignedBox(Vector<T, n>(), Vector<T, n>());
		Vector<T, n> minima = in_points[0];
		Vector<T, n> maxima = in_points[0];
		for (uint i = 1; i < in_nPoints; i++) {
			for (uint j = 0; j < n; j++) {
				if (in_points[i].Get(j) < minima.Get(j)) minima[j] = in_points[i].Get(j);
				if (in_points[i].Get(j) > maxima.Get(j)) maxima[j] = in_points[i].Get(j);
			}
		}
		return AxisAlignedBox(minima, maxima);
	}
};

template<class T>
using AxisAlignedRectangle = AxisAlignedBox<T, 2>;
using AxisAlignedRectanglef = AxisAlignedRectangle<float>;
using AxisAlignedRectangled = AxisAlignedRectangle<double>;
using AxisAlignedBoxf = AxisAlignedBox<float, 3>;
using AxisAlignedBoxd = AxisAlignedBox<double, 3>;

#endif