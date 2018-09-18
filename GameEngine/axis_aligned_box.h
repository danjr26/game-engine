#ifndef AXIS_ALIGNED_BOX_H
#define AXIS_ALIGNED_BOX_H

#include "transform.h"

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
				corners[i][j] = ((i >> j) & 1) ? maxima[j] : minima[j];
			}
		}
	}

	void Apply_Transform(const Transform& transform) {
		const uint nCorners = 1 << n;

		Vector<T, n> corners[nCorners];
		Get_Corners(corners);

		for (uint i = 0; i < nCorners; i++) {
			corners[i] = Vector<T, n>(transform.Apply_To_Local_Point(Vector3d(corners[i])));
		}

		(*this) = AxisAlignedBox<T, n>::From_Bounded_Points(nCorners, corners);
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

	Vector<T, n> Get_Dimensions() const {
		return maxima - minima;
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