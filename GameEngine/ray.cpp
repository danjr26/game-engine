#include "ray.h"
#include "exceptions.h"

template<class T, uint n>
inline Ray<T, n>::Ray(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
	mPoint(in_point),
	mDirection(in_direction.Normalized()) {}

template<class T, uint n>
inline void Ray<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	mPoint = in_transform.Local_To_World_Point(mPoint);
	mDirection = in_transform.Local_To_World_Direction(mDirection);
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::Get_Point() const {
	return mPoint;
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::Get_Direction() const {
	return mDirection;
}

template<class T, uint n>
inline T Ray<T, n>::Get_Projection_Coefficient() const {
	return mDirection.Dot(mPoint);
}

template<class T, uint n>
inline T Ray<T, n>::Get_Projection_Coefficient(const Vector<T, n>& in_point) const {
	return (in_point - mPoint).Projection_Coeff(mDirection);
}

template<class T, uint n>
inline Vector<T, n> Ray<T, n>::Get_Projection(const Vector<T, n>& in_point) const {
	return (in_point - mPoint).Projection(mDirection) + mPoint;
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