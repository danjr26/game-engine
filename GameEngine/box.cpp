#include "box.h"

template<class T, uint n>
inline Box<T, n>::Box(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes) :
	mOrigin(in_origin) {

	for (uint i = 0; i < n; i++) mAxes[i] = in_axes[i];
}

template<class T, uint n>
Box<T, n>::Box() :
	mOrigin(),
	mAxes()
{}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::getOrigin() const {
	return mOrigin;
}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::getCenter() const {
	Vector<T, n> out = mOrigin;
	for (uint i = 0; i < n; i++) out += mAxes[i] / 2.0;
	return out;
}

template<class T, uint n>
inline void Box<T, n>::getAxes(Vector<T, n>* out_axes) const {
	for (uint i = 0; i < n; i++) out_axes[i] = mAxes[i];
}

template<class T, uint n>
inline void Box<T, n>::getCorners(Vector<T, n>* out_corners) const {
	const uint nCorners = 1 << n;

	for (uint i = 0; i < nCorners; i++) {
		out_corners[i] = mOrigin;
		for (uint j = 0; j < n; j++) {
			if ((i >> j) & 1) out_corners[i] += mAxes[j];
		}
	}
}

template<class T, uint n>
inline void Box<T, n>::applyTransform(const Transform<T, n>& transform) {
	mOrigin = transform.localToWorldPoint(mOrigin);
	for (uint i = 0; i < n; i++) {
		mAxes[i] = transform.localToWorldVector(mAxes[i]);
	}
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::getArea() const {
	return mAxes[0].magnitude() * mAxes[1].magnitude();
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::getPerimeter() const {
	return (mAxes[0].magnitude() + mAxes[1].magnitude()) * 2.0;
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::getVolume() const {
	return mAxes[0].magnitude() * mAxes[1].magnitude() * mAxes[2].magnitude();
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::getSurfaceArea() const {
	Vector<T, n> edgeLengths = {
		mAxes[0].magnitude(),
		mAxes[1].magnitude(),
		mAxes[2].magnitude()
	};
	return (
		edgeLengths.x() * edgeLengths.y() +
		edgeLengths.y() * edgeLengths.z() +
		edgeLengths.z() * edgeLengths.x()
		) * 2.0;
}

template<class T, uint n>
template<typename>
inline T Box<T, n>::getTotalEdgeLength() const {
	return (mAxes[0].magnitude() + mAxes[1].magnitude() + mAxes[2].magnitude()) * 2.0;
}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::randomPointBoundary() const {
	Vector<T, n> out = mOrigin;
	uint stuckDimension = GEUtil::random<uint>(n);
	for (uint i = 0; i < n; i++) {
		if (i == stuckDimension) {
			out += (GEUtil::random<bool>()) ? Vector<T, n>() : mAxes[i];
		}
		else {
			out += mAxes[i] * GEUtil::random<T>(1);
		}
	}
	return out;
}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::randomPointInside() const {
	Vector<T, n> out = mOrigin;
	for (uint i = 0; i < n; i++) {
		out += mAxes[i] * GEUtil::random<T>(1);
	}
	return out;
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::fromCenterAxes(const Vector<T, n>& in_center, const Vector<T, n>* in_axes) {
	Vector<T, n> origin = in_center;
	for (uint i = 0; i < n; i++) origin -= in_axes[i] / 2.0;
	return Box<T, n>(origin, in_axes);
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::fromCenterAxes(const Vector<T, n>& in_center, std::initializer_list<Vector<T, n>> in_axes) {
	Vector<T, n> origin = in_center;
	for (uint i = 0; i < n; i++) origin -= in_axes.begin()[i] / 2.0;
	return Box<T, n>(origin, in_axes.begin());
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::fromOriginAxes(const Vector<T, n>& in_origin, const Vector<T, n>* in_axes) {
	return Box<T, n>(in_origin, in_axes);
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::fromOriginAxes(const Vector<T, n>& in_origin, std::initializer_list<Vector<T, n>> in_axes) {
	return Box<T, n>(in_origin, in_axes.begin());
}

template class Box<float, 2>;
template class Box<double, 2>;
template class Box<float, 3>;
template class Box<double, 3>;