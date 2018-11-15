#include "triangle.h"
#include "exceptions.h"

template<class T, uint n>
inline Triangle<T, n>::Triangle(const Vector<T, n>* in_points) {
	for (uint i = 0; i < 3; i++) points[i] = in_points[i];
}

template<class T, uint n>
inline void Triangle<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	for (uint i = 0; i < 3; i++) {
		points[i] = in_transform.Local_To_World_Point(points[i]);
	}
}

template<class T, uint n>
inline void Triangle<T, n>::Get_Points(Vector<T, n>* out_points) {
	memcpy(out_points, points, sizeof(Vector<T, n>) * 3);
}

template<class T, uint n>
inline void Triangle<T, n>::Get_Point_Offsets(Vector<T, n>* out_offsets) {
	for (uint i = 0; i < 3; i++) {
		out_offsets[i] = points[(i + 1) % 3];
		out_offsets[i] -= points[i];
	}
}

template<class T, uint n>
Vector<T, n>& Triangle<T, n>::operator[](uint in_index) {
	return points[in_index];
}

template<class T, uint n>
inline Vector<T, n> Triangle<T, n>::Get_Point(uint in_index) const {
	return points[in_index];
}

template<class T, uint n>
inline T Triangle<T, n>::Get_Perimeter() const {
	T out = 0;
	for (uint i = 0; i < 3; i++) out += (points[(i + 1) % 3] - points[i]).Magnitude();
	return out;
}

template<class T, uint n>
Vector<T, n> Triangle<T, n>::Random_Point_Boundary() const {
	uint i = Random<uint>(3);
	return points[i] + (points[(i + 1) % 3] - points[i]) * Random<T>(1);
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::Random_Point_Inside() const {
	Vector<T, n> out = (points[1] - points[0]) * Random<T>(1) + (points[2] - points[0]) * Random<T>(1);
	Vector<T, n> farNormal = (points[1] - points[2]).Orthogonal().Normalized();

	T outProjectionCoeff = out.Projection_Coeff(farNormal);
	T segmentProjectionCoeff = points[1].Projection_Coeff(farNormal);

	if (outProjectionCoeff > segmentProjectionCoeff) {
		outProjectionCoeff = segmentProjectionCoeff - (outProjectionCoeff - segmentProjectionCoeff);
		out = farNormal * outProjectionCoeff;
	}

	return out;
}

template<class T, uint n>
template<typename, typename>
Vector<T, n> Triangle<T, n>::Random_Point_Inside() const {
	Vector<T, n> out = (points[1] - points[0]) * Random<T>(1) + (points[2] - points[0]) * Random<T>(1);
	Vector<T, n> farNormal = Get_Lazy_Normal().Cross(points[1] - points[2]).Normalized();

	T outProjectionCoeff = out.Projection_Coeff(farNormal);
	T segmentProjectionCoeff = points[1].Projection_Coeff(farNormal);

	if (outProjectionCoeff > segmentProjectionCoeff) {
		outProjectionCoeff = segmentProjectionCoeff - (outProjectionCoeff - segmentProjectionCoeff);
		out = farNormal * outProjectionCoeff;
	}

	return out;
}

template<class T, uint n>
template<typename>
inline void Triangle<T, n>::Get_Lazy_Normals(Vector<T, n>* out_lazyNormals) {
	for (uint i = 0; i < 3; i++) out_lazyNormals[i] = (points[i] - points[(i + 1) % 3]).Orthogonal();
}

template void Triangle<float, 2>::Get_Lazy_Normals(Vector2f*);
template void Triangle<double, 2>::Get_Lazy_Normals(Vector2d*);

template<class T, uint n>
template<typename>
inline void Triangle<T, n>::Get_Normals(Vector<T, n>* out_Normals) {
	for (uint i = 0; i < 3; i++) out_Normals[i] = (points[i] - points[(i + 1) % 3]).Orthogonal().Normalized();
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::Get_Lazy_Normal() const {
	return (points[1] - points[0]).Cross(points[2] - points[0]);
}

template<class T, uint n>
template<typename>
Vector<T, n> Triangle<T, n>::Get_Normal() const {
	return (points[1] - points[0]).Cross(points[2] - points[0]).Normalized();
}

template void Triangle<float, 2>::Get_Normals(Vector2f*);
template void Triangle<double, 2>::Get_Normals(Vector2d*);

template<class T, uint n>
template<typename>
inline T Triangle<T, n>::Get_Area() const {
	T edgeDots[3] = {
		(points[1] - points[0]).Dot_Self(),
		(points[2] - points[1]).Dot_Self(),
		(points[0] - points[2]).Dot_Self(),
	};

	T term1 = edgeDots[0] + edgeDots[1] + edgeDots[2];
	T term2 = edgeDots[0] * edgeDots[0] + edgeDots[1] * edgeDots[1] + edgeDots[2] * edgeDots[2];

	return 0.25 * sqrt(term1 * term1 - 2 * term2);
}

template<class T, uint n>
template<typename, typename>
inline T Triangle<T, n>::Get_Area() const {
	return 0.5 * (points[1] - points[0]).Cross(points[2] - points[0]).Magnitude();
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