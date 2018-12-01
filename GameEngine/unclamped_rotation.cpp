#include "unclamped_rotation.h"
#include "rotation.h"

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation() :
	angle(0)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(T in_angle) :
	angle(in_angle)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(T in_from, T in_to) :
	angle(in_to - in_from) 
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(const UnclampedRotation<T, 2>& in_from, const UnclampedRotation<T, 2>& in_to) :
	angle(in_to.angle - in_from.angle)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(const Rotation<T, 2>& in_rotation) :
	angle(in_rotation.Get_Angle())
{}

template<class T>
bool UnclampedRotation<T, 2>::Is_Identity() const {
	return angle == 0;
}

template<class T>
void UnclampedRotation<T, 2>::Invert() {
	angle = -angle;
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::Get_Inverse() const {
	return UnclampedRotation<T, 2>(-angle);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator+(const UnclampedRotation<T, 2>& in_rotation) const {
	return UnclampedRotation<T, 2>(angle + in_rotation.angle);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator-(const UnclampedRotation<T, 2>& in_rotation) const {
	return UnclampedRotation<T, 2>(angle - in_rotation.angle);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator*(T in_scalar) const {
	return UnclampedRotation<T, 2>(angle * in_scalar);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator/(T in_scalar) const {
	return UnclampedRotation<T, 2>(angle / in_scalar);
}

template<class T>
void UnclampedRotation<T, 2>::operator+=(const UnclampedRotation<T, 2>& in_rotation) {
	angle += in_rotation.angle;
}

template<class T>
void UnclampedRotation<T, 2>::operator-=(const UnclampedRotation<T, 2>& in_rotation) {
	angle -= in_rotation.angle;
}

template<class T>
void UnclampedRotation<T, 2>::operator*=(T in_scalar) {
	angle *= in_scalar;
}

template<class T>
void UnclampedRotation<T, 2>::operator/=(T in_scalar) {
	angle /= in_scalar;
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::Lerp(const UnclampedRotation<T, 2>& in_rotation, T t) const {
	return UnclampedRotation<T, 2>(angle * (1 - t) + in_rotation.angle * t);
}

template<class T>
T UnclampedRotation<T, 2>::Get_Angle() const {
	return angle;
}

template<class T>
Vector<T, 2> UnclampedRotation<T, 2>::Apply_To(const Vector<T, 2>& in_point) const {
	return in_point.Rotated(angle);
}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation() :
	axis(Vector<T, 3>(0, 0, -1)),
	angle(0) 
{}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation(const Vector<T, 3>& in_axis, T in_angle) :
	axis(in_axis.Normalized()),
	angle(in_angle) 
{}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation(const Vector<T, 3>& in_from, const Vector<T, 3>& in_to) :
	axis(in_from.Cross(in_to).Normalized()),
	angle(in_from.Theta(in_to)) {
	if (abs(angle - (T)PI) < (T)PI / 10000.0) {
		if (abs(in_from.Dot(Vector<T, 3>(0, 1, 0)) > 0.9)) {
			axis = in_from.Cross(Vector<T, 3>(1, 0, 0)).Normalized();
		}
		else {
			axis = in_from.Cross(Vector<T, 3>(0, 1, 0)).Normalized();
		}
		angle = (T)PI;
	}
}

template<class T>
UnclampedRotation<T, 3>::UnclampedRotation(const Rotation<T, 3>& in_rotation) :
	axis(in_rotation.Get_Axis()),
	angle(in_rotation.Get_Angle())
{}

template<class T>
inline bool UnclampedRotation<T, 3>::Is_Identity() const {
	return angle == 0;
}

template<class T>
inline void UnclampedRotation<T, 3>::Invert() {
	angle = -angle;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::Get_Inverse() const {
	return UnclampedRotation<T, 3>(axis, -angle);
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::operator+(const UnclampedRotation<T, 3>& in_rotation) const {
	UnclampedRotation<T, 3> out = *this;
	out += in_rotation;
	return out;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::operator-(const UnclampedRotation<T, 3>& in_rotation) const {
	UnclampedRotation<T, 3> out = *this;
	out -= in_rotation;
	return out;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::operator*(T in_scalar) const {
	UnclampedRotation<T, 3> out = *this;
	out *= in_scalar;
	return out;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::operator/(T in_scalar) const {
	UnclampedRotation<T, 3> out = *this;
	out /= in_scalar;
	return out;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator+=(const UnclampedRotation<T, 3>& in_rotation) {
	axis = axis * angle + in_rotation.axis * in_rotation.angle;
	angle = axis.Magnitude();
	axis /= angle;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator-=(const UnclampedRotation<T, 3>& in_rotation) {
	axis = axis * angle - in_rotation.axis * in_rotation.angle;
	angle = axis.Magnitude();
	axis /= angle;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator*=(T in_scalar) {
	angle *= in_scalar;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator/=(T in_scalar) {
	angle /= in_scalar;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::Lerp(const UnclampedRotation<T, 3>& in_rotation, T t) const {
	return *this * (1 - t) + in_rotation * t;
}

template<class T>
inline T UnclampedRotation<T, 3>::Get_Angle() const {
	return angle;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::With_Angle(T in_angle) const {
	return UnclampedRotation<T, 3>(axis, in_angle);
}

template<class T>
inline Vector<T, 3> UnclampedRotation<T, 3>::Get_Axis() const {
	return axis;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::With_Axis(const Vector<T, 3>& in_axis) const {
	return UnclampedRotation<T, 3>(in_axis, angle);
}

template<class T>
inline Vector<T, 3> UnclampedRotation<T, 3>::Apply_To(const Vector<T, 3>& in_point) const {
	return in_point.Rotated(axis, angle);
}

template class UnclampedRotation<float, 2>;
template class UnclampedRotation<double, 2>;
template class UnclampedRotation<float, 3>;
template class UnclampedRotation<double, 3>;
