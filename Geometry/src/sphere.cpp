#include "../include/internal/sphere.h"
#include "../include/internal/triangle.h"

template<class T, uint n>
inline Sphere<T, n>::Sphere(const Vector<T, n>& i_center, T i_radius) :
	mCenter(i_center),
	mRadius(i_radius) {}

template<class T, uint n>
Sphere<T, n>::Sphere() :
	mCenter(),
	mRadius(0)
{}

template<class T, uint n>
inline void Sphere<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	T scale = 1.0;
	for (Transform<T, n>const* t = &i_transform; t != nullptr; t = t->getConstParent()) {
		scale *= t->getLocalScale().compMax();
	}
	(*this) = Sphere<T, n>::fromPointRadius(
		i_transform.localToWorldPoint(mCenter),
		mRadius * scale
	);
}

template<class T, uint n>
void Sphere<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	Vector<T, n> normOffset = (io_request.mPoint - mCenter).normalized();

	if (io_request.wantsPoint()) {
		io_request.mPoint = mCenter + normOffset * mRadius;
	}

	if (io_request.wantsNormal()) {
		io_request.mNormal = normOffset;
	}

	if (n == 2) {
		io_request.mContactType = ClosestRequest<T, n>::on_edge;
	}
	else if (n == 3) {
		io_request.mContactType = ClosestRequest<T, n>::on_face;
	}
}

template<class T, uint n>
inline T Sphere<T, n>::getRadius() const {
	return mRadius;
}

template<class T, uint n>
inline void Sphere<T, n>::setRadius(T i_radius) {
	mRadius = i_radius;
}

template<class T, uint n>
inline Vector<T, n> Sphere<T, n>::getCenter() const {
	return mCenter;
}

template<class T, uint n>
inline void Sphere<T, n>::setCenter(const Vector<T, n>& i_center) {
	mCenter = i_center;
}

template<class T, uint n>
T Sphere<T, n>::getLazyRadius() const {
	return mRadius * mRadius;
}

template<class T, uint n>
inline Vector<T, n> Sphere<T, n>::getExtrema(uint i_dimension, bool i_isPositive) const {
	if (i_dimension >= n) fail();
	Vector<T, n> out = mCenter;
	out[i_dimension] += (i_isPositive) ? mRadius : -mRadius;
	return out;
}

template<class T, uint n>
Range<T> Sphere<T, n>::getRange(uint i_dimension) const {
	T centerValue = mCenter[i_dimension];
	return Range<T>(centerValue - mRadius, centerValue + mRadius);
}

template<class T, uint n>
template<typename>
Vector<T, n> Sphere<T, n>::randomPointBoundary() const {
	T angle = random<T>((T)(2 * PI));
	return Vector<T, n>(cos(angle), sin(angle)) * mRadius;
}

template<class T, uint n>
template<typename, typename>
Vector<T, n> Sphere<T, n>::randomPointBoundary() const {
	T angle = random<T>((T)(2 * PI));
	T z = random<T>(-1, 1);
	T u = sqrt(1 - z * z);
	return mCenter + Vector<T, n>(u * cos(angle), u * sin(angle), z) * mRadius;
}

template<class T, uint n>
Sphere<T, n> Sphere<T, n>::fromBoundedPoints(Vector<T, n>* i_points, uint i_nPoints) {
	std::set<Vector<T, n>> points(i_points, i_points + i_nPoints);
	std::set<Vector<T, n>> boundaryPoints;
	return welzl(points, boundaryPoints);
}

template<class T, uint n>
Sphere<T, n> Sphere<T, n>::fromBoundedTriangle(const Triangle<T, n>& i_triangle) {
	for (uint i = 0; i < 3; i++) {
		if (i_triangle.getAngle(i) >= PI / 2) {
			Vector<T, n> center = (i_triangle.getPoint((i + 1) % 3) + i_triangle.getPoint((i + 2) % 3)) * (T)0.5;
			T radius = (i_triangle.getPoint((i + 1) % 3) - center).magnitude();
			return Sphere<T, n>(center, radius);
		}
	}

	return Sphere<T, n>(i_triangle.getCircumcenter(), i_triangle.getCircumradius());
}

template<class T, uint n>
Sphere<T, n> Sphere<T, n>::welzl(std::set<Vector<T, n>>& i_points, std::set<Vector<T, n>>& i_borderPoints) {
	if (i_points.empty() || i_borderPoints.size() >= 3) {
		Vector<T, n> radius;
		auto it = i_borderPoints.begin();
		switch (i_borderPoints.size()) {
		case 0:
			return { Vector<T, n>(), (T)0 };
		case 1:
			return { *i_borderPoints.begin(), (T)0 };
		case 2:
			radius = (*i_borderPoints.end() - *i_borderPoints.begin()) * 0.5;
			return { *i_borderPoints.begin() + radius, radius.magnitude() };
		default:
			return Sphere<T, n>::fromBoundedTriangle(Triangle<T, n>::fromPoints(*(it++), *(it++), *(it++)));
		}
	}
	Vector<T, n> point = *i_points.begin();
	i_points.erase(i_points.begin());
	Sphere<T, n> lastSphere = welzl(i_points, i_borderPoints);
	if ((point - lastSphere.getCenter()).dotSelf() <= lastSphere.getLazyRadius()) {
		return lastSphere;
	}
	i_borderPoints.insert(point);
	Sphere<T, n> newSphere = welzl(i_points, i_borderPoints);
	i_borderPoints.erase(i_borderPoints.find(point));
	i_points.insert(point);
	return newSphere;
}

template<class T, uint n>
Vector<T, n> Sphere<T, n>::randomPointInside() const {
	return (randomPointBoundary() - mCenter) * sqrt(random<T>(1)) + mCenter;
}

template<class T, uint n>
inline Sphere<T, n> Sphere<T, n>::fromPointRadius(const Vector<T, n>& i_center, T i_radius) {
	return Sphere(i_center, i_radius);
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::getArea() const {
	return PI * mRadius * mRadius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::getCircumference() const {
	return 2.0 * PI * mRadius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::getVolume() const {
	return 4.0 / 3.0 * PI * mRadius * mRadius * mRadius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::getSurfaceArea() const {
	return 4.0 * PI * mRadius * mRadius;
}

template<class T, uint n>
template<typename>
inline T Sphere<T, n>::getMajorCircumference() const {
	return 2.0 * PI * mRadius;
}

template class Sphere<float, 2>;
template class Sphere<double, 2>;
template class Sphere<float, 3>;
template class Sphere<double, 3>;