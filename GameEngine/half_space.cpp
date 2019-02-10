#include "half_space.h"
#include "exceptions.h"

template<class T, uint n>
HalfSpace<T, n>::HalfSpace(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) :
	mPoint(in_point),
	mDirection(in_direction.Normalized()) {}

template<class T, uint n>
void HalfSpace<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	mPoint = in_transform.Local_To_World_Point(mPoint);
	mDirection = in_transform.Local_To_World_Direction(mDirection);
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::Get_Point() const {
	return mPoint;
}

template<class T, uint n>
void HalfSpace<T, n>::Set_Point(const Vector<T, n>& in_point) {
	mPoint = in_point;
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::Get_Direction() const {
	return mDirection;
}

template<class T, uint n>
void HalfSpace<T, n>::Set_Direction(const Vector<T, n>& in_direction) {
	mDirection = in_direction.Normalized();
}

template<class T, uint n>
T HalfSpace<T, n>::Get_Projection_Coefficient() const {
	return mPoint.Dot(mDirection);
}

template<class T, uint n>
Vector<T, n> HalfSpace<T, n>::Get_Projection() const {
	return mPoint.Dot(mDirection) * mDirection;
}

template<class T, uint n>
HalfSpace<T, n> HalfSpace<T, n>::From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
	return HalfSpace<T, n>(in_point, in_direction);
}

template class HalfSpace<float, 2>;
template class HalfSpace<double, 2>;
template class HalfSpace<float, 3>;
template class HalfSpace<double, 3>;