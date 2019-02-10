#include "axis_aligned_box.h"
#include "exceptions.h"

template<class T, uint n>
inline AxisAlignedBox<T, n>::AxisAlignedBox(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima) :
	mMinima(in_minima),
	mMaxima(in_maxima) {}

template<class T, uint n>
inline void AxisAlignedBox<T, n>::getCorners(Vector<T, n>* out_corners) const {
	const uint nCorners = 1 << n;

	for (uint i = 0; i < nCorners; i++) {
		for (uint j = 0; j < n; j++) {
			out_corners[i][j] = ((i >> j) & 1) ? mMaxima[j] : mMinima[j];
		}
	}
}

template<class T, uint n>
inline void AxisAlignedBox<T, n>::applyTransform(const Transform<T, n>& transform) {
	if (transform.getWorldRotation().isIdentity()) {
		for (Transform<T, n> const* t = &transform; t != nullptr; t = t->getConstParent()) {
			mMinima = mMinima.compwise(t->getLocalScale());
			mMinima += t->getLocalPosition();

			mMaxima = mMaxima.compwise(t->getLocalScale());
			mMaxima += t->getLocalPosition();
		}
		return;
	}

	const uint nCorners = 1 << n;

	Vector<T, n> corners[nCorners];
	getCorners(corners);

	for (uint i = 0; i < nCorners; i++) {
		corners[i] = transform.localToWorldPoint(corners[i]);
	}

	(*this) = AxisAlignedBox<T, n>::fromBoundedPoints(nCorners, corners);
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::getCenter() const {
	return (mMinima + mMaxima) / 2;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::getDimensions() const {
	return mMaxima - mMinima;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::getMinima() const {
	return mMinima;
}

template<class T, uint n>
inline void AxisAlignedBox<T, n>::setMinima(const Vector<T, n>& in_minima) {
	mMinima = in_minima;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::getMaxima() const {
	return mMaxima;
}

template<class T, uint n>
inline void AxisAlignedBox<T, n>::setMaxima(const Vector<T, n>& in_maxima) {
	mMaxima = in_maxima;
}

template<class T, uint n>
inline Range<T> AxisAlignedBox<T, n>::getRange(uint dimension) const {
	if (dimension >= n) {
		throw InvalidArgumentException();
	}
	return Range<T>(mMinima[dimension], mMaxima[dimension]);
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::randomPointBoundary() const {
	Vector<T, n> out;
	uint stuckDimension = GEUtil::random<uint>(n);
	for (uint i = 0; i < n; i++) {
		if (i == stuckDimension) {
			out[i] = (GEUtil::random<bool>()) ? mMinima.get(i) : mMaxima.get(i);
		}
		else {
			out[i] = GEUtil::random<T>(mMinima.get(i), mMaxima.get(i));
		}
	}
	return out;
}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::getArea() const {
	return (mMaxima - mMinima).componentProduct();
}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::getPerimeter() const {
	return (mMaxima - mMinima).componentSum() * 2.0;
}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::getVolume() const {
	return (mMaxima - mMinima).componentProduct();

}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::getSurfaceArea() const {
	Vector<T, n> edgeLengths = mMaxima - mMinima;
	return (
		edgeLengths.x() * edgeLengths.y() +
		edgeLengths.y() * edgeLengths.z() +
		edgeLengths.z() * edgeLengths.x()
		) * 2.0;
}

template<class T, uint n>
template<typename>
inline T AxisAlignedBox<T, n>::getTotalEdgeLength() const {
	return (mMaxima - mMinima).componentSum() * 2.0;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedBox<T, n>::randomPointInside() const {
	Vector<T, n> out;
	for (uint i = 0; i < n; i++) {
		out[i] = GEUtil::random<T>(mMinima.get(i), mMaxima.get(i));
	}
	return out;
}

template<class T, uint n>
inline AxisAlignedBox<T, n> AxisAlignedBox<T, n>::fromExtrema(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima) {
	return AxisAlignedBox(in_minima, in_maxima);
}

template<class T, uint n>
inline AxisAlignedBox<T, n> AxisAlignedBox<T, n>::fromCenter(const Vector<T, n>& in_center, const Vector<T, n>& in_dimensions) {
	const Vector<T, n> halfDimensions(in_dimensions / 2.0);
	return AxisAlignedBox(in_center - halfDimensions, in_center + halfDimensions);
}

template<class T, uint n>
inline AxisAlignedBox<T, n> AxisAlignedBox<T, n>::fromBoundedPoints(uint in_nPoints, const Vector<T, n>* in_points) {
	if (in_nPoints == 0) return AxisAlignedBox(Vector<T, n>(), Vector<T, n>());
	Vector<T, n> minima = in_points[0];
	Vector<T, n> maxima = in_points[0];
	for (uint i = 1; i < in_nPoints; i++) {
		for (uint j = 0; j < n; j++) {
			if (in_points[i].get(j) < minima.get(j)) minima[j] = in_points[i].get(j);
			if (in_points[i].get(j) > maxima.get(j)) maxima[j] = in_points[i].get(j);
		}
	}
	return AxisAlignedBox(minima, maxima);
}

template class AxisAlignedBox<float, 2>;
template class AxisAlignedBox<double, 2>;
template class AxisAlignedBox<float, 3>;
template class AxisAlignedBox<double, 3>;