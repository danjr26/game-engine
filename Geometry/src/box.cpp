#include "../include/internal/box.h"

template<class T, uint n>
inline Box<T, n>::Box(const Vector<T, n>& i_origin, const Vector<T, n>* i_axes) :
	mOrigin(i_origin) {

	for (uint i = 0; i < n; i++) mAxes[i] = i_axes[i];
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
void Box<T, n>::getAxes(Vector<T, n>* o_axes) const {
	for (uint i = 0; i < n; i++) o_axes[i] = mAxes[i];
}

template<class T, uint n>
void Box<T, n>::getNormalAxes(Vector<T, n>* o_axes) const {
	for (uint i = 0; i < n; i++) o_axes[i] = mAxes[i].normalized();
}

template<class T, uint n>
inline void Box<T, n>::getCorners(Vector<T, n>* o_corners) const {
	const uint nCorners = 1 << n;

	for (uint i = 0; i < nCorners; i++) {
		o_corners[i] = mOrigin;
		for (uint j = 0; j < n; j++) {
			if ((i >> j) & 1) o_corners[i] += mAxes[j];
		}
	}
}

template<class T, uint n>
inline void Box<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	mOrigin = i_transform.localToWorldPoint(mOrigin);
	for (uint i = 0; i < n; i++) {
		mAxes[i] = i_transform.localToWorldVector(mAxes[i]);
	}
}

template<class T, uint n>
void Box<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	Vector<T, n> point = io_request.mPoint;
	Vector<T, n> center = getCenter();
	Vector<T, n> normAxes[n];
	getNormalAxes(normAxes);

	io_request.mPoint = mOrigin;
	io_request.mNormal = Vector<T, n>();

	uint nOutside = 0;
	for (uint i = 0; i < n; i++) {
		T p = point.projectionCoeff(normAxes[i]);
		T c = center.projectionCoeff(normAxes[i]);
		T lowEnd = mOrigin.projectionCoeff(normAxes[i]);
		T highEnd = (mOrigin + mAxes[i]).projectionCoeff(normAxes[i]);

		if (io_request.wantsPoint()) {
			io_request.mPoint += normAxes[i] * closest(p, lowEnd, highEnd);
		}

		if (io_request.wantsNormal()) {
			io_request.mNormal += normAxes[i] * (T)compare(p, c);
		}

		if (!betwInc(p, lowEnd, highEnd)) nOutside++;
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
	uint stuckDimension = random<uint>(n);
	for (uint i = 0; i < n; i++) {
		if (i == stuckDimension) {
			out += (random<bool>()) ? Vector<T, n>() : mAxes[i];
		}
		else {
			out += mAxes[i] * random<T>(1);
		}
	}
	return out;
}

template<class T, uint n>
inline Vector<T, n> Box<T, n>::randomPointInside() const {
	Vector<T, n> out = mOrigin;
	for (uint i = 0; i < n; i++) {
		out += mAxes[i] * random<T>(1);
	}
	return out;
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::fromCenterAxes(const Vector<T, n>& i_center, const Vector<T, n>* i_axes) {
	Vector<T, n> origin = i_center;
	for (uint i = 0; i < n; i++) origin -= i_axes[i] / 2.0;
	return Box<T, n>(origin, i_axes);
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::fromCenterAxes(const Vector<T, n>& i_center, std::initializer_list<Vector<T, n>> i_axes) {
	Vector<T, n> origin = i_center;
	for (uint i = 0; i < n; i++) origin -= i_axes.begin()[i] / 2.0;
	return Box<T, n>(origin, i_axes.begin());
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::fromOriginAxes(const Vector<T, n>& i_origin, const Vector<T, n>* i_axes) {
	return Box<T, n>(i_origin, i_axes);
}

template<class T, uint n>
inline Box<T, n> Box<T, n>::fromOriginAxes(const Vector<T, n>& i_origin, std::initializer_list<Vector<T, n>> i_axes) {
	return Box<T, n>(i_origin, i_axes.begin());
}

template class Box<float, 2>;
template class Box<double, 2>;
template class Box<float, 3>;
template class Box<double, 3>;