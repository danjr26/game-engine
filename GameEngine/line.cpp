#include "line.h"
#include "exceptions.h"

template<class T, uint n>
inline Line<T, n>::Line(const Vector<T, n>& in_point, const Vector<T, n> in_direction) :
	point(in_point),
	direction(in_direction.Normalized()) {}

template<class T, uint n>
inline void Line<T, n>::Apply_Transform(Transform<T, n>& in_transform) {
	point = in_transform.Apply_To_Local_Point(point);
	direction = in_transform.Apply_To_Local_Direction(direction);
}

template<class T, uint n>
inline Vector<T, n> Line<T, n>::Get_Point() const {
	return point;
}

template<class T, uint n>
inline Vector<T, n> Line<T, n>::Get_Direction() const {
	return direction;
}

template<class T, uint n>
T Line<T, n>::Get_Projection_Coefficient() const {
	return point.Projection_Coeff(direction);
}

template<class T, uint n>
inline T Line<T, n>::Get_Projection_Coefficient(const Vector<T, n>& in_point) const {
	return in_point.Projection_Coeff(direction);
}

template<class T, uint n>
inline Vector<T, n> Line<T, n>::Get_Projection(const Vector<T, n>& in_point) const {
	return (in_point - point).Projection(direction) + point;
}

template<class T, uint n>
T Line<T, n>::Distance_To(const Vector<T, n>& in_point) const {
	return (Get_Projection(in_point) - in_point).Magnitude();
}

template<class T, uint n>
Vector<T, n> Line<T, n>::Flip(const Vector<T, n>& in_point) const {
	Vector<T, n> projection = Get_Projection(in_point);
	return in_point + (projection - in_point) * 2;
}

template<class T, uint n>
inline Line<T, n> Line<T, n>::From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2) {
	return Line<T, n>(in_point1, in_point2 - in_point1);
}

template<class T, uint n>
inline Line<T, n> Line<T, n>::From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
	return Line<T, n>(in_point, in_direction);
}

template class Line<float, 2>;
template class Line<double, 2>;
template class Line<float, 3>;
template class Line<double, 3>;