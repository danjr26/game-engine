#include "../include/internal/triangle.h"

template<class T, uint n>
Triangle<T, n>::Triangle(const Vector<T, n>* i_points) {
	for (uint i = 0; i < 3; i++) mPoints[i] = i_points[i];
}

template<class T, uint n>
Triangle<T, n>::Triangle() 
{}

template<class T, uint n>
void Triangle<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	for (uint i = 0; i < 3; i++) {
		mPoints[i] = i_transform.localToWorldPoint(mPoints[i]);
	}
}

template<class T, uint n>
void Triangle<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	if constexpr (n == 2) {
		Vector<T, n> point = io_request.mPoint;
		Vector<T, n> normals[3];
		getNormals(normals);
		Vector<T, n> incenter = getIncenter();

		T edgeDots[3];
		T radialDots[3];
		for (uint i = 0; i < 3; i++) {
			edgeDots[i] = (point - mPoints[i]).dot(normals[i]);
			radialDots[i] = (point - mPoints[i]).dot((mPoints[i] - incenter).orthogonal());
		}

		for (uint i = 0; i < 3; i++) {
			if (edgeDots[i] >= 0 && edgeDots[(i + 2) % 3] >= 0) {
				if (io_request.wantsPoint()) io_request.mPoint = mPoints[i];
				if (io_request.wantsNormal()) io_request.mNormal = point - mPoints[i];
				io_request.mContactType = ClosestRequest<T, n>::on_point;
			}
		}

		for (uint i = 0; i < 3; i++) {
			if (radialDots[i] >= 0 && radialDots[(i + 1) % 3] <= 0) {
				if (io_request.wantsPoint()) io_request.mPoint = mPoints[i] + (point - mPoints[i]).projection(getPointOffset(i));
				if (io_request.wantsNormal()) io_request.mNormal = normals[i];
				io_request.mContactType = ClosestRequest<T, n>::on_edge;
			}
		}
	}

	if constexpr (n == 3) {
		//TODO
	}
}

template<class T, uint n>
inline void Triangle<T, n>::getPoints(Vector<T, n>* o_points) const {
	memcpy(o_points, mPoints, sizeof(Vector<T, n>) * 3);
}

template<class T, uint n>
inline void Triangle<T, n>::getPointOffsets(Vector<T, n>* o_offsets) const {
	for (uint i = 0; i < 3; i++) {
		o_offsets[i] = mPoints[(i + 1) % 3];
		o_offsets[i] -= mPoints[i];
	}
}

template<class T, uint n>
T Triangle<T, n>::getAngle(uint i_index) const {
	return (mPoints[(i_index + 1) % 3] - mPoints[i_index]).theta(mPoints[(i_index + 2) % 3] - mPoints[i_index]);
}

template<class T, uint n>
T Triangle<T, n>::getSideLength(uint i_index) const {
	return getPointOffset(i_index).magnitude();
}

template<class T, uint n>
Vector<T, n>& Triangle<T, n>::operator[](uint i_index) {
	return mPoints[i_index];
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::getPoint(uint i_index) const {
	return mPoints[i_index];
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::getPointOffset(uint i_index) const {
	return mPoints[(i_index + 1) % 3] - mPoints[i_index];
}

template<class T, uint n>
inline T Triangle<T, n>::getPerimeter() const {
	T out = 0;
	for (uint i = 0; i < 3; i++) out += (mPoints[(i + 1) % 3] - mPoints[i]).magnitude();
	return out;
}

template<class T, uint n>
T Triangle<T, n>::getCircumradius() const {
	T a = getSideLength(0);
	T b = getSideLength(1);
	T c = getSideLength(2);

	return (a * b * c) / sqrt((a + b + c) * (b + c - a) * (a + c - b) * (a + b - c));
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::getCircumcenter() const {
	Vector<T, n> out;

	T total = 0;
	T radius = getCircumradius();
	for (uint i = 0; i < 3; i++) {
		T cosAngle = cos(getAngle(i));
		T trilinear = radius * cosAngle;
		T barycentric = trilinear * getSideLength(i);
		out += barycentric * mPoints[i];
		total += barycentric;
	}
	out /= total;

	return out;
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::getIncenter() const {
	Vector<T, n> out;
	T perimeter = 0;
	for (uint i = 0; i < 3; i++) {
		T length = getSideLength((i + 1) % 3);
		perimeter += length;
		out += mPoints[i] * length;
	}
	return out / perimeter;
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::randomPointBoundary() const {
	uint i = random<uint>(3);
	return mPoints[i] + (mPoints[(i + 1) % 3] - mPoints[i]) * random<T>(1);
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::randomPointInside() const {
	Vector<T, n> out = (mPoints[1] - mPoints[0]) * random<T>(1) + (mPoints[2] - mPoints[0]) * random<T>(1);
	Vector<T, n> farNormal = (mPoints[1] - mPoints[2]).orthogonal().normalized();

	T outProjectionCoeff = out.projectionCoeff(farNormal);
	T segmentProjectionCoeff = mPoints[1].projectionCoeff(farNormal);

	if (outProjectionCoeff > segmentProjectionCoeff) {
		outProjectionCoeff = segmentProjectionCoeff - (outProjectionCoeff - segmentProjectionCoeff);
		out = farNormal * outProjectionCoeff;
	}

	return out;
}

template<class T, uint n>
template<typename, typename>
Vector<T, n> Triangle<T, n>::randomPointInside() const {
	Vector<T, n> out = (mPoints[1] - mPoints[0]) * random<T>(1) + (mPoints[2] - mPoints[0]) * random<T>(1);
	Vector<T, n> farNormal = getLazyNormal().cross(mPoints[1] - mPoints[2]).normalized();

	T outProjectionCoeff = out.projectionCoeff(farNormal);
	T segmentProjectionCoeff = mPoints[1].projectionCoeff(farNormal);

	if (outProjectionCoeff > segmentProjectionCoeff) {
		outProjectionCoeff = segmentProjectionCoeff - (outProjectionCoeff - segmentProjectionCoeff);
		out = farNormal * outProjectionCoeff;
	}

	return out;
}

template<class T, uint n>
template<typename>
inline void Triangle<T, n>::getLazyNormals(Vector<T, n>* o_lazyNormals) const {
	for (uint i = 0; i < 3; i++) o_lazyNormals[i] = (mPoints[i] - mPoints[(i + 1) % 3]).orthogonal();
}

template void Triangle<float, 2>::getLazyNormals(Vector2f*) const;
template void Triangle<double, 2>::getLazyNormals(Vector2d*) const;

template<class T, uint n>
template<typename>
inline void Triangle<T, n>::getNormals(Vector<T, n>* o_Normals) const {
	for (uint i = 0; i < 3; i++) o_Normals[i] = (mPoints[i] - mPoints[(i + 1) % 3]).orthogonal().normalized();
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::getLazyNormal() const {
	return (mPoints[1] - mPoints[0]).cross(mPoints[2] - mPoints[0]);
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::getNormal() const {
	return (mPoints[1] - mPoints[0]).cross(mPoints[2] - mPoints[0]).normalized();
}

template void Triangle<float, 2>::getNormals(Vector2f*) const;
template void Triangle<double, 2>::getNormals(Vector2d*) const;

template<class T, uint n>
template<typename>
inline T Triangle<T, n>::getArea() const {
	T edgeDots[3] = {
		(mPoints[1] - mPoints[0]).dotSelf(),
		(mPoints[2] - mPoints[1]).dotSelf(),
		(mPoints[0] - mPoints[2]).dotSelf(),
	};

	T term1 = edgeDots[0] + edgeDots[1] + edgeDots[2];
	T term2 = edgeDots[0] * edgeDots[0] + edgeDots[1] * edgeDots[1] + edgeDots[2] * edgeDots[2];

	return 0.25 * sqrt(term1 * term1 - 2 * term2);
}

template<class T, uint n>
template<typename, typename>
inline T Triangle<T, n>::getArea() const {
	return 0.5 * (mPoints[1] - mPoints[0]).cross(mPoints[2] - mPoints[0]).magnitude();
}

template<class T, uint n>
inline Triangle<T, n> Triangle<T, n>::fromPoints(const Vector<T, n>* i_points) {
	return Triangle<T, n>(i_points);
}

template<class T, uint n>
inline Triangle<T, n> Triangle<T, n>::fromPoints(const Vector<T, n>& i_point1, const Vector<T, n>& i_point2, const Vector<T, n>& i_point3) {
	Vector<T, n> points[3] = { i_point1, i_point2, i_point3 };
	return Triangle<T, n>(points);
}

template class Triangle<float, 2>;
template class Triangle<double, 2>;
template class Triangle<float, 3>;
template class Triangle<double, 3>;