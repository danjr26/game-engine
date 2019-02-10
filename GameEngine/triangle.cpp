#include "triangle.h"
#include "exceptions.h"

template<class T, uint n>
inline Triangle<T, n>::Triangle(const Vector<T, n>* in_points) {
	for (uint i = 0; i < 3; i++) mPoints[i] = in_points[i];
}

template<class T, uint n>
Triangle<T, n>::Triangle() 
{}

template<class T, uint n>
inline void Triangle<T, n>::applyTransform(const Transform<T, n>& in_transform) {
	for (uint i = 0; i < 3; i++) {
		mPoints[i] = in_transform.localToWorldPoint(mPoints[i]);
	}
}

template<class T, uint n>
inline void Triangle<T, n>::getPoints(Vector<T, n>* out_points) const {
	memcpy(out_points, mPoints, sizeof(Vector<T, n>) * 3);
}

template<class T, uint n>
inline void Triangle<T, n>::getPointOffsets(Vector<T, n>* out_offsets) const {
	for (uint i = 0; i < 3; i++) {
		out_offsets[i] = mPoints[(i + 1) % 3];
		out_offsets[i] -= mPoints[i];
	}
}

template<class T, uint n>
T Triangle<T, n>::getAngle(uint in_index) const {
	return (mPoints[(in_index + 1) % 3] - mPoints[in_index]).theta(mPoints[(in_index + 2) % 3] - mPoints[in_index]);
}

template<class T, uint n>
T Triangle<T, n>::getSideLength(uint in_index) const {
	return getPointOffset(in_index).magnitude();
}

template<class T, uint n>
Vector<T, n>& Triangle<T, n>::operator[](uint in_index) {
	return mPoints[in_index];
}

template<class T, uint n>
inline Vector<T, n> Triangle<T, n>::getPoint(uint in_index) const {
	return mPoints[in_index];
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::getPointOffset(uint in_index) const {
	return mPoints[(in_index + 1) % 3] - mPoints[in_index];
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
Vector<T, n> Triangle<T, n>::randomPointBoundary() const {
	uint i = GEUtil::random<uint>(3);
	return mPoints[i] + (mPoints[(i + 1) % 3] - mPoints[i]) * GEUtil::random<T>(1);
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::randomPointInside() const {
	Vector<T, n> out = (mPoints[1] - mPoints[0]) * GEUtil::random<T>(1) + (mPoints[2] - mPoints[0]) * GEUtil::random<T>(1);
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
	Vector<T, n> out = (mPoints[1] - mPoints[0]) * GEUtil::random<T>(1) + (mPoints[2] - mPoints[0]) * GEUtil::random<T>(1);
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
inline void Triangle<T, n>::getLazyNormals(Vector<T, n>* out_lazyNormals) const {
	for (uint i = 0; i < 3; i++) out_lazyNormals[i] = (mPoints[i] - mPoints[(i + 1) % 3]).orthogonal();
}

template void Triangle<float, 2>::getLazyNormals(Vector2f*) const;
template void Triangle<double, 2>::getLazyNormals(Vector2d*) const;

template<class T, uint n>
template<typename>
inline void Triangle<T, n>::getNormals(Vector<T, n>* out_Normals) const {
	for (uint i = 0; i < 3; i++) out_Normals[i] = (mPoints[i] - mPoints[(i + 1) % 3]).orthogonal().normalized();
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
inline Triangle<T, n> Triangle<T, n>::fromPoints(const Vector<T, n>* in_points) {
	return Triangle<T, n>(in_points);
}

template<class T, uint n>
inline Triangle<T, n> Triangle<T, n>::fromPoints(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2, const Vector<T, n>& in_point3) {
	Vector<T, n> points[3] = { in_point1, in_point2, in_point3 };
	return Triangle<T, n>(points);
}

template class Triangle<float, 2>;
template class Triangle<double, 2>;
template class Triangle<float, 3>;
template class Triangle<double, 3>;