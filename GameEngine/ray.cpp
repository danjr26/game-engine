#include "ray.h"
#include "exceptions.h"

template<class T, uint n>
inline Ray<T, n>::Ray(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
	point(in_point),
	direction(in_direction.Normalized()) {}

template<class T, uint n>
inline void Ray<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	point = in_transform.Local_To_World_Point(point);
	direction = in_transform.Local_To_World_Direction(direction);
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::Get_Point() const {
	return point;
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::Get_Direction() const {
	return direction;
}

template<class T, uint n>
inline T Ray<T, n>::Get_Projection_Coefficient() const {
	return direction.Dot(point);
}

template<class T, uint n>
inline T Ray<T, n>::Get_Projection_Coefficient(const Vector<T, n>& in_point) {
	return (in_point - point).Projection_Coeff(direction);
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::Get_Projection(const Vector<T, n>& in_point) {
	return (in_point - point).Projection(direction) + point;
}

template<class T, uint n>
inline Ray<T, n> Ray<T, n>::From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
	return Ray<T, n>(in_point, in_direction);
}

template<class T, uint n>
inline Ray<T, n> Ray<T, n>::From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2) {
	return Ray<T, n>(in_point1, in_point2 - in_point1);
}

template class Ray<float, 2>;
template class Ray<double, 2>;
template class Ray<float, 3>;
template class Ray<double, 3>;