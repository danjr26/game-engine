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
inline void Triangle<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	for (uint i = 0; i < 3; i++) {
		mPoints[i] = in_transform.Local_To_World_Point(mPoints[i]);
	}
}

template<class T, uint n>
inline void Triangle<T, n>::Get_Points(Vector<T, n>* out_points) const {
	memcpy(out_points, mPoints, sizeof(Vector<T, n>) * 3);
}

template<class T, uint n>
inline void Triangle<T, n>::Get_Point_Offsets(Vector<T, n>* out_offsets) const {
	for (uint i = 0; i < 3; i++) {
		out_offsets[i] = mPoints[(i + 1) % 3];
		out_offsets[i] -= mPoints[i];
	}
}

template<class T, uint n>
T Triangle<T, n>::Get_Angle(uint in_index) const {
	return (mPoints[(in_index + 1) % 3] - mPoints[in_index]).Theta(mPoints[(in_index + 2) % 3] - mPoints[in_index]);
}

template<class T, uint n>
T Triangle<T, n>::Get_Side_Length(uint in_index) const {
	return Get_Point_Offset(in_index).Magnitude();
}

template<class T, uint n>
Vector<T, n>& Triangle<T, n>::operator[](uint in_index) {
	return mPoints[in_index];
}

template<class T, uint n>
inline Vector<T, n> Triangle<T, n>::Get_Point(uint in_index) const {
	return mPoints[in_index];
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::Get_Point_Offset(uint in_index) const {
	return mPoints[(in_index + 1) % 3] - mPoints[in_index];
}

template<class T, uint n>
inline T Triangle<T, n>::Get_Perimeter() const {
	T out = 0;
	for (uint i = 0; i < 3; i++) out += (mPoints[(i + 1) % 3] - mPoints[i]).Magnitude();
	return out;
}

template<class T, uint n>
T Triangle<T, n>::Get_Circumradius() const {
	T a = Get_Side_Length(0);
	T b = Get_Side_Length(1);
	T c = Get_Side_Length(2);

	return (a * b * c) / sqrt((a + b + c) * (b + c - a) * (a + c - b) * (a + b - c));
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::Get_Circumcenter() const {
	Vector<T, n> out;

	T total = 0;
	T radius = Get_Circumradius();
	for (uint i = 0; i < 3; i++) {
		T cosAngle = cos(Get_Angle(i));
		T trilinear = radius * cosAngle;
		T barycentric = trilinear * Get_Side_Length(i);
		out += barycentric * mPoints[i];
		total += barycentric;
	}
	out /= total;

	return out;
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::Random_Point_Boundary() const {
	uint i = Random<uint>(3);
	return mPoints[i] + (mPoints[(i + 1) % 3] - mPoints[i]) * Random<T>(1);
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::Random_Point_Inside() const {
	Vector<T, n> out = (mPoints[1] - mPoints[0]) * Random<T>(1) + (mPoints[2] - mPoints[0]) * Random<T>(1);
	Vector<T, n> farNormal = (mPoints[1] - mPoints[2]).Orthogonal().Normalized();

	T outProjectionCoeff = out.Projection_Coeff(farNormal);
	T segmentProjectionCoeff = mPoints[1].Projection_Coeff(farNormal);

	if (outProjectionCoeff > segmentProjectionCoeff) {
		outProjectionCoeff = segmentProjectionCoeff - (outProjectionCoeff - segmentProjectionCoeff);
		out = farNormal * outProjectionCoeff;
	}

	return out;
}

template<class T, uint n>
template<typename, typename>
Vector<T, n> Triangle<T, n>::Random_Point_Inside() const {
	Vector<T, n> out = (mPoints[1] - mPoints[0]) * Random<T>(1) + (mPoints[2] - mPoints[0]) * Random<T>(1);
	Vector<T, n> farNormal = Get_Lazy_Normal().Cross(mPoints[1] - mPoints[2]).Normalized();

	T outProjectionCoeff = out.Projection_Coeff(farNormal);
	T segmentProjectionCoeff = mPoints[1].Projection_Coeff(farNormal);

	if (outProjectionCoeff > segmentProjectionCoeff) {
		outProjectionCoeff = segmentProjectionCoeff - (outProjectionCoeff - segmentProjectionCoeff);
		out = farNormal * outProjectionCoeff;
	}

	return out;
}

template<class T, uint n>
template<typename>
inline void Triangle<T, n>::Get_Lazy_Normals(Vector<T, n>* out_lazyNormals) const {
	for (uint i = 0; i < 3; i++) out_lazyNormals[i] = (mPoints[i] - mPoints[(i + 1) % 3]).Orthogonal();
}

template void Triangle<float, 2>::Get_Lazy_Normals(Vector2f*) const;
template void Triangle<double, 2>::Get_Lazy_Normals(Vector2d*) const;

template<class T, uint n>
template<typename>
inline void Triangle<T, n>::Get_Normals(Vector<T, n>* out_Normals) const {
	for (uint i = 0; i < 3; i++) out_Normals[i] = (mPoints[i] - mPoints[(i + 1) % 3]).Orthogonal().Normalized();
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::Get_Lazy_Normal() const {
	return (mPoints[1] - mPoints[0]).Cross(mPoints[2] - mPoints[0]);
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::Get_Normal() const {
	return (mPoints[1] - mPoints[0]).Cross(mPoints[2] - mPoints[0]).Normalized();
}

template void Triangle<float, 2>::Get_Normals(Vector2f*) const;
template void Triangle<double, 2>::Get_Normals(Vector2d*) const;

template<class T, uint n>
template<typename>
inline T Triangle<T, n>::Get_Area() const {
	T edgeDots[3] = {
		(mPoints[1] - mPoints[0]).Dot_Self(),
		(mPoints[2] - mPoints[1]).Dot_Self(),
		(mPoints[0] - mPoints[2]).Dot_Self(),
	};

	T term1 = edgeDots[0] + edgeDots[1] + edgeDots[2];
	T term2 = edgeDots[0] * edgeDots[0] + edgeDots[1] * edgeDots[1] + edgeDots[2] * edgeDots[2];

	return 0.25 * sqrt(term1 * term1 - 2 * term2);
}

template<class T, uint n>
template<typename, typename>
inline T Triangle<T, n>::Get_Area() const {
	return 0.5 * (mPoints[1] - mPoints[0]).Cross(mPoints[2] - mPoints[0]).Magnitude();
}

template<class T, uint n>
inline Triangle<T, n> Triangle<T, n>::From_Points(const Vector<T, n>* in_points) {
	return Triangle<T, n>(in_points);
}

template<class T, uint n>
inline Triangle<T, n> Triangle<T, n>::From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2, const Vector<T, n>& in_point3) {
	Vector<T, n> points[3] = { in_point1, in_point2, in_point3 };
	return Triangle<T, n>(points);
}

template class Triangle<float, 2>;
template class Triangle<double, 2>;
template class Triangle<float, 3>;
template class Triangle<double, 3>;