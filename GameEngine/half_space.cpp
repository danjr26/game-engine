#include "half_space.h"
#include "exceptions.h"

template<class T, uint n>
HalfSpace<T, n>::HalfSpace(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
	point(in_point),
	direction(in_direction.Normalized()) {}

template<class T, uint n>
void HalfSpace<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	point = in_transform.Local_To_World_Point(point);
	direction = in_transform.Local_To_World_Direction(direction);
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::Get_Point() const {
	return point;
}

template<class T, uint n>
void HalfSpace<T, n>::Set_Point(const Vector<T, n>& in_point) {
	point = in_point;
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::Get_Direction() const {
	return direction;
}

template<class T, uint n>
void HalfSpace<T, n>::Set_Direction(const Vector<T, n>& in_direction) {
	direction = in_direction.Normalized();
}

template<class T, uint n>
T HalfSpace<T, n>::Get_Projection_Coefficient() const {
	return point.Dot(direction);
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::Get_Projection() const {
	return point.Dot(direction) * direction;
}

template<class T, uint n>
HalfSpace<T, n> HalfSpace<T, n>::From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
	return HalfSpace<T, n>(in_point, in_direction);
}

template class HalfSpace<float, 2>;
template class HalfSpace<double, 2>;
template class HalfSpace<float, 3>;
template class HalfSpace<double, 3>;