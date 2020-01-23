#include "../include/internal/axis_aligned_box.h"
#include "../include/internal/sphere.h"

template<class T, uint n>
inline AABox<T, n>::AABox(const Vector<T, n>& i_minima, const Vector<T, n>& i_maxima) :
	mMinima(i_minima),
	mMaxima(i_maxima) 
{}

template<class T, uint n>
inline void AABox<T, n>::getCorners(Vector<T, n>* o_corners) const {
	const uint nCorners = 1 << n;

	for (uint i = 0; i < nCorners; i++) {
		for (uint j = 0; j < n; j++) {
			o_corners[i][j] = ((i >> j) & 1) ? mMaxima[j] : mMinima[j];
		}
	}
}

template<class T, uint n>
AABox<T, n>::AABox() :
	mMinima(),
	mMaxima()
{}

template<class T, uint n>
inline void AABox<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	if (i_transform.getWorldRotation().isIdentity()) {
		for (Transform<T, n> const* t = &i_transform; t != nullptr; t = t->getConstParent()) {
			mMinima = mMinima.compMult(t->getLocalScale());
			mMinima += t->getLocalPosition();

			mMaxima = mMaxima.compMult(t->getLocalScale());
			mMaxima += t->getLocalPosition();
		}
		return;
	}

	const uint nCorners = 1 << n;

	Vector<T, n> corners[nCorners];
	getCorners(corners);

	for (uint i = 0; i < nCorners; i++) {
		corners[i] = i_transform.localToWorldPoint(corners[i]);
	}

	(*this) = AABox<T, n>::fromBoundedPoints(nCorners, corners);
}

template<class T, uint n>
void AABox<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	Vector<T, n> point = io_request.mPoint;
	Vector<T, n> center = getCenter();

	uint nOutside = 0;
	for (uint i = 0; i < n; i++) {
		if (io_request.wantsPoint()) {
			io_request.mPoint[i] = closest(point[i], mMinima[i], mMaxima[i]);
		}

		if (io_request.wantsNormal()) {
			io_request.mNormal[i] = (T)compare(point[i], center[i]);
		}

		if (!betwInc(point[i], mMinima[i], mMaxima[i])) nOutside++;
	}

	if (io_request.wantsNormal()) {
		io_request.mNormal.normalize();
	}

	if (n == 2) {
		if (nOutside == 2) io_request.mContactType = ClosestRequest<T, n>::on_point;
		else io_request.mContactType = ClosestRequest<T, n>::on_edge;
	}
	else if (n == 3) {
		if (nOutside == 3) io_request.mContactType = ClosestRequest<T, n>::on_point;
		if (nOutside == 2) io_request.mContactType = ClosestRequest<T, n>::on_edge;
		else io_request.mContactType = ClosestRequest<T, n>::on_face;
	}
}

template<class T, uint n>
inline Vector<T, n> AABox<T, n>::getCenter() const {
	return (mMinima + mMaxima) / 2;
}

template<class T, uint n>
inline Vector<T, n> AABox<T, n>::getDimensions() const {
	return mMaxima - mMinima;
}

template<class T, uint n>
inline Vector<T, n> AABox<T, n>::getMinima() const {
	return mMinima;
}

template<class T, uint n>
inline void AABox<T, n>::setMinima(const Vector<T, n>& i_minima) {
	mMinima = i_minima;
}

template<class T, uint n>
inline Vector<T, n> AABox<T, n>::getMaxima() const {
	return mMaxima;
}

template<class T, uint n>
inline void AABox<T, n>::setMaxima(const Vector<T, n>& i_maxima) {
	mMaxima = i_maxima;
}

template<class T, uint n>
inline Range<T> AABox<T, n>::getRange(uint dimension) const {
	if (dimension >= n) fail();
	return Range<T>(mMinima[dimension], mMaxima[dimension]);
}

template<class T, uint n>
bool AABox<T, n>::containsPoint(const Vector<T, n>& i_point) const {
	for (uint i = 0; i < n; i++) {
		if (i_point[i] < mMinima[i] || i_point[i] > mMaxima[i]) return false;
	}
	return true;
}

template<class T, uint n>
inline Vector<T, n> AABox<T, n>::randomPointBoundary() const {
	Vector<T, n> out;
	uint stuckDimension = random<uint>(n);
	for (uint i = 0; i < n; i++) {
		if (i == stuckDimension) {
			out[i] = (random<bool>()) ? mMinima.get(i) : mMaxima.get(i);
		}
		else {
			out[i] = random<T>(mMinima.get(i), mMaxima.get(i));
		}
	}
	return out;
}

template<class T, uint n>
template<typename>
inline T AABox<T, n>::getArea() const {
	return (mMaxima - mMinima).componentProduct();
}

template<class T, uint n>
template<typename>
inline T AABox<T, n>::getPerimeter() const {
	return (mMaxima - mMinima).componentSum() * 2.0;
}

template<class T, uint n>
template<typename>
inline T AABox<T, n>::getVolume() const {
	return (mMaxima - mMinima).componentProduct();

}

template<class T, uint n>
template<typename>
inline T AABox<T, n>::getSurfaceArea() const {
	Vector<T, n> edgeLengths = mMaxima - mMinima;
	return (
		edgeLengths.x() * edgeLengths.y() +
		edgeLengths.y() * edgeLengths.z() +
		edgeLengths.z() * edgeLengths.x()
		) * 2.0;
}

template<class T, uint n>
template<typename>
inline T AABox<T, n>::getTotalEdgeLength() const {
	return (mMaxima - mMinima).componentSum() * 2.0;
}

template<class T, uint n>
inline Vector<T, n> AABox<T, n>::randomPointInside() const {
	Vector<T, n> out;
	for (uint i = 0; i < n; i++) {
		out[i] = random<T>(mMinima.get(i), mMaxima.get(i));
	}
	return out;
}

template<class T, uint n>
inline AABox<T, n> AABox<T, n>::fromExtrema(const Vector<T, n>& i_minima, const Vector<T, n>& i_maxima) {
	return AABox(i_minima, i_maxima);
}

template<class T, uint n>
AABox<T, n> AABox<T, n>::fromRanges(const Range<T>* i_ranges) {
	Vector<T, n> minima, maxima;
	for (uint i = 0; i < n; i++) {
		minima[i] = i_ranges[i].getLow();
		maxima[i] = i_ranges[i].getHigh();
	}
	return AABox(minima, maxima);
}

template<class T, uint n>
inline AABox<T, n> AABox<T, n>::fromCenter(const Vector<T, n>& i_center, const Vector<T, n>& i_dimensions) {
	const Vector<T, n> halfDimensions(i_dimensions / (T)2);
	return AABox(i_center - halfDimensions, i_center + halfDimensions);
}

template<class T, uint n>
inline AABox<T, n> AABox<T, n>::fromBoundedPoints(uint i_nPoints, const Vector<T, n>* i_points) {
	if (i_nPoints == 0) return AABox(Vector<T, n>(), Vector<T, n>());
	Vector<T, n> minima = i_points[0];
	Vector<T, n> maxima = i_points[0];
	for (uint i = 1; i < i_nPoints; i++) {
		for (uint j = 0; j < n; j++) {
			if (i_points[i].get(j) < minima.get(j)) minima[j] = i_points[i].get(j);
			if (i_points[i].get(j) > maxima.get(j)) maxima[j] = i_points[i].get(j);
		}
	}
	return AABox(minima, maxima);
}

template<class T, uint n>
AABox<T, n> AABox<T, n>::fromBoundedSphere(const Sphere<T, n>& i_sphere) {
	Vector<T, n> center = i_sphere.getCenter();
	Vector<T, n> taxiRadius = Vector<T, n>::filled(i_sphere.getRadius());
	return AABox(center - taxiRadius, center + taxiRadius);
}

template class AABox<float, 2>;
template class AABox<double, 2>;
template class AABox<float, 3>;
template class AABox<double, 3>;