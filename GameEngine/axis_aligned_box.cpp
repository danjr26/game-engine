#include "axis_aligned_box.h"
#include "exceptions.h"

template<class T, uint n>
inline AxisAlignedBox<T, n>::AxisAlignedBox(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima) :
	minima(in_minima),
	maxima(in_maxima) {}

template<class T, uint n>
inline void AxisAlignedBox<T, n>::Get_Corners(Vector<T, n>* out_corners) {
	const uint nCorners = 1 << n;

	for (uint i = 0; i < nCorners; i++) {
		for (uint j = 0; j < n; j++) {
			out_corners[i][j] = ((i >> j) & 1) ? maxima[j] : minima[j];
		}
	}
}

template<class T, uint n>
inline void AxisAlignedBox<T, n>::Apply_Transform(Transform<T, n>& transform) {
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

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::Get_Center() const {
	return (minima + maxima) / 2;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::Get_Dimensions() const {
	return maxima - minima;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::Get_Minima() const {
	return minima;
}

template<class T, uint n>
inline void AxisAlignedBox<T, n>::Set_Minima(const Vector<T, n>& in_minima) {
	minima = in_minima;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::Get_Maxima() const {
	return maxima;
}

template<class T, uint n>
inline void AxisAlignedBox<T, n>::Set_Maxima(const Vector<T, n>& in_maxima) {
	maxima = in_maxima;
}

template<class T, uint n>
inline Range<T> AxisAlignedBox<T, n>::Get_Range(uint dimension) {
	if (dimension >= n) {
		throw InvalidArgumentException();
	}
	return Range<T>(minima[dimension], maxima[dimension]);
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::Random_Point_Boundary() const {
	Vector<T, n> out;
	uint stuckDimension = Random<uint>(n);
	for (uint i = 0; i < n; i++) {
		if (i == stuckDimension) {
			out[i] = (Random<bool>()) ? minima.Get(i) : maxima.Get(i);
		}
		else {
			out[i] = Random<T>(minima.Get(i), maxima.Get(i));
		}
	}
	return out;
}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::Get_Area() const {
	return (maxima - minima).Component_Product();
}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::Get_Perimeter() const {
	return (maxima - minima).Component_Sum() * 2.0;
}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::Get_Volume() const {
	return (maxima - minima).Component_Product();

}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::Get_Surface_Area() const {
	Vector<T, n> edgeLengths = maxima - minima;
	return (
		edgeLengths.X() * edgeLengths.Y() +
		edgeLengths.Y() * edgeLengths.Z() +
		edgeLengths.Z() * edgeLengths.X()
		) * 2.0;
}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::Get_Total_Edge_Length() const {
	return (maxima - minima).Component_Sum() * 2.0;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::Random_Point_Inside() const {
	Vector<T, n> out;
	for (uint i = 0; i < n; i++) {
		out[i] = Random<T>(minima.Get(i), maxima.Get(i));
	}
	return out;
}

template<class T, uint n>
inline AxisAlignedBox<T, n> AxisAlignedBox<T, n>::From_Extrema(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima) {
	return AxisAlignedBox(in_minima, in_maxima);
}

template<class T, uint n>
inline AxisAlignedBox<T, n> AxisAlignedBox<T, n>::From_Center(const Vector<T, n>& in_center, const Vector<T, n>& in_dimensions) {
	const Vector<T, n> halfDimensions(in_dimensions / 2.0);
	return AxisAlignedBox(in_center - halfDimensions, in_center + halfDimensions);
}

template<class T, uint n>
inline AxisAlignedBox<T, n> AxisAlignedBox<T, n>::From_Bounded_Points(uint in_nPoints, const Vector<T, n>* in_points) {
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

template class AxisAlignedBox<float, 2>;
template class AxisAlignedBox<double, 2>;
template class AxisAlignedBox<float, 3>;
template class AxisAlignedBox<double, 3>;