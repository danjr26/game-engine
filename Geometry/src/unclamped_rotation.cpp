#include "../include/internal/unclamped_rotation.h"
#include "../include/internal/rotation.h"

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation() :
	mAngle(0)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(T i_angle) :
	mAngle(i_angle)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(T i_from, T i_to) :
	mAngle(i_to - i_from) 
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(const UnclampedRotation<T, 2>& i_from, const UnclampedRotation<T, 2>& i_to) :
	mAngle(i_to.mAngle - i_from.mAngle)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(const Rotation<T, 2>& i_rotation) :
	mAngle(i_rotation.getAngle())
{}

template<class T>
bool UnclampedRotation<T, 2>::isIdentity() const {
	return mAngle == 0;
}

template<class T>
void UnclampedRotation<T, 2>::invert() {
	mAngle = -mAngle;
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::getInverse() const {
	return UnclampedRotation<T, 2>(-mAngle);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator+(const UnclampedRotation<T, 2>& i_rotation) const {
	return UnclampedRotation<T, 2>(mAngle + i_rotation.mAngle);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator-(const UnclampedRotation<T, 2>& i_rotation) const {
	return UnclampedRotation<T, 2>(mAngle - i_rotation.mAngle);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator*(T i_scalar) const {
	return UnclampedRotation<T, 2>(mAngle * i_scalar);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator/(T i_scalar) const {
	return UnclampedRotation<T, 2>(mAngle / i_scalar);
}

template<class T>
void UnclampedRotation<T, 2>::operator+=(const UnclampedRotation<T, 2>& i_rotation) {
	mAngle += i_rotation.mAngle;
}

template<class T>
void UnclampedRotation<T, 2>::operator-=(const UnclampedRotation<T, 2>& i_rotation) {
	mAngle -= i_rotation.mAngle;
}

template<class T>
void UnclampedRotation<T, 2>::operator*=(T i_scalar) {
	mAngle *= i_scalar;
}

template<class T>
void UnclampedRotation<T, 2>::operator/=(T i_scalar) {
	mAngle /= i_scalar;
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::lerp(const UnclampedRotation<T, 2>& i_rotation, T t) const {
	return UnclampedRotation<T, 2>(mAngle * (1 - t) + i_rotation.mAngle * t);
}

template<class T>
T UnclampedRotation<T, 2>::getAngle() const {
	return mAngle;
}

template<class T>
Vector<T, 2> UnclampedRotation<T, 2>::applyTo(const Vector<T, 2>& i_point) const {
	return i_point.rotated(mAngle);
}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation() :
	axis(Vector<T, 3>(0, 0, -1)),
	angle(0) 
{}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation(const Vector<T, 3>& i_axis, T i_angle) :
	axis(i_axis.normalized()),
	angle(i_angle) 
{}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation(const Vector<T, 3>& i_from, const Vector<T, 3>& i_to) :
	axis(i_from.cross(i_to).normalized()),
	angle(i_from.theta(i_to)) {
	if (abs(angle - (T)PI) < (T)PI / 10000.0) {
		if (abs(i_from.dot(Vector<T, 3>(0, 1, 0)) > 0.9)) {
			axis = i_from.cross(Vector<T, 3>(1, 0, 0)).normalized();
		}
		else {
			axis = i_from.cross(Vector<T, 3>(0, 1, 0)).normalized();
		}
		angle = (T)PI;
	}
}

template<class T>
UnclampedRotation<T, 3>::UnclampedRotation(const Rotation<T, 3>& i_rotation) :
	axis(i_rotation.getAxis()),
	angle(i_rotation.getAngle())
{}

template<class T>
inline bool UnclampedRotation<T, 3>::isIdentity() const {
	return angle == 0;
}

template<class T>
inline void UnclampedRotation<T, 3>::invert() {
	angle = -angle;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::getInverse() const {
	return UnclampedRotation<T, 3>(axis, -angle);
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::operator+(const UnclampedRotation<T, 3>& i_rotation) const {
	UnclampedRotation<T, 3> out = *this;
	out += i_rotation;
	return out;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::operator-(const UnclampedRotation<T, 3>& i_rotation) const {
	UnclampedRotation<T, 3> out = *this;
	out -= i_rotation;
	return out;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::operator*(T i_scalar) const {
	UnclampedRotation<T, 3> out = *this;
	out *= i_scalar;
	return out;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::operator/(T i_scalar) const {
	UnclampedRotation<T, 3> out = *this;
	out /= i_scalar;
	return out;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator+=(const UnclampedRotation<T, 3>& i_rotation) {
	axis = axis * angle + i_rotation.axis * i_rotation.angle;
	angle = axis.magnitude();
	axis /= angle;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator-=(const UnclampedRotation<T, 3>& i_rotation) {
	axis = axis * angle - i_rotation.axis * i_rotation.angle;
	angle = axis.magnitude();
	axis /= angle;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator*=(T i_scalar) {
	angle *= i_scalar;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator/=(T i_scalar) {
	angle /= i_scalar;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::lerp(const UnclampedRotation<T, 3>& i_rotation, T t) const {
	return *this * (1 - t) + i_rotation * t;
}

template<class T>
inline T UnclampedRotation<T, 3>::getAngle() const {
	return angle;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::withAngle(T i_angle) const {
	return UnclampedRotation<T, 3>(axis, i_angle);
}

template<class T>
inline Vector<T, 3> UnclampedRotation<T, 3>::getAxis() const {
	return axis;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::withAxis(const Vector<T, 3>& i_axis) const {
	return UnclampedRotation<T, 3>(i_axis, angle);
}

template<class T>
inline Vector<T, 3> UnclampedRotation<T, 3>::applyTo(const Vector<T, 3>& i_point) const {
	return i_point.rotated(axis, angle);
}

template class UnclampedRotation<float, 2>;
template class UnclampedRotation<double, 2>;
template class UnclampedRotation<float, 3>;
template class UnclampedRotation<double, 3>;
