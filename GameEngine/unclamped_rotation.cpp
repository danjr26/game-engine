#include "unclamped_rotation.h"
#include "rotation.h"

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation() :
	mAngle(0)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(T in_angle) :
	mAngle(in_angle)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(T in_from, T in_to) :
	mAngle(in_to - in_from) 
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(const UnclampedRotation<T, 2>& in_from, const UnclampedRotation<T, 2>& in_to) :
	mAngle(in_to.mAngle - in_from.mAngle)
{}

template<class T>
UnclampedRotation<T, 2>::UnclampedRotation(const Rotation<T, 2>& in_rotation) :
	mAngle(in_rotation.getAngle())
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
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator+(const UnclampedRotation<T, 2>& in_rotation) const {
	return UnclampedRotation<T, 2>(mAngle + in_rotation.mAngle);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator-(const UnclampedRotation<T, 2>& in_rotation) const {
	return UnclampedRotation<T, 2>(mAngle - in_rotation.mAngle);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator*(T in_scalar) const {
	return UnclampedRotation<T, 2>(mAngle * in_scalar);
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::operator/(T in_scalar) const {
	return UnclampedRotation<T, 2>(mAngle / in_scalar);
}

template<class T>
void UnclampedRotation<T, 2>::operator+=(const UnclampedRotation<T, 2>& in_rotation) {
	mAngle += in_rotation.mAngle;
}

template<class T>
void UnclampedRotation<T, 2>::operator-=(const UnclampedRotation<T, 2>& in_rotation) {
	mAngle -= in_rotation.mAngle;
}

template<class T>
void UnclampedRotation<T, 2>::operator*=(T in_scalar) {
	mAngle *= in_scalar;
}

template<class T>
void UnclampedRotation<T, 2>::operator/=(T in_scalar) {
	mAngle /= in_scalar;
}

template<class T>
UnclampedRotation<T, 2> UnclampedRotation<T, 2>::lerp(const UnclampedRotation<T, 2>& in_rotation, T t) const {
	return UnclampedRotation<T, 2>(mAngle * (1 - t) + in_rotation.mAngle * t);
}

template<class T>
T UnclampedRotation<T, 2>::getAngle() const {
	return mAngle;
}

template<class T>
Vector<T, 2> UnclampedRotation<T, 2>::applyTo(const Vector<T, 2>& in_point) const {
	return in_point.rotated(mAngle);
}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation() :
	axis(Vector<T, 3>(0, 0, -1)),
	angle(0) 
{}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation(const Vector<T, 3>& in_axis, T in_angle) :
	axis(in_axis.normalized()),
	angle(in_angle) 
{}

template<class T>
inline UnclampedRotation<T, 3>::UnclampedRotation(const Vector<T, 3>& in_from, const Vector<T, 3>& in_to) :
	axis(in_from.cross(in_to).normalized()),
	angle(in_from.theta(in_to)) {
	if (abs(angle - (T)PI) < (T)PI / 10000.0) {
		if (abs(in_from.dot(Vector<T, 3>(0, 1, 0)) > 0.9)) {
			axis = in_from.cross(Vector<T, 3>(1, 0, 0)).normalized();
		}
		else {
			axis = in_from.cross(Vector<T, 3>(0, 1, 0)).normalized();
		}
		angle = (T)PI;
	}
}

template<class T>
UnclampedRotation<T, 3>::UnclampedRotation(const Rotation<T, 3>& in_rotation) :
	axis(in_rotation.getAxis()),
	angle(in_rotation.getAngle())
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
	angle = axis.magnitude();
	axis /= angle;
}

template<class T>
inline void UnclampedRotation<T, 3>::operator-=(const UnclampedRotation<T, 3>& in_rotation) {
	axis = axis * angle - in_rotation.axis * in_rotation.angle;
	angle = axis.magnitude();
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
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::lerp(const UnclampedRotation<T, 3>& in_rotation, T t) const {
	return *this * (1 - t) + in_rotation * t;
}

template<class T>
inline T UnclampedRotation<T, 3>::getAngle() const {
	return angle;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::withAngle(T in_angle) const {
	return UnclampedRotation<T, 3>(axis, in_angle);
}

template<class T>
inline Vector<T, 3> UnclampedRotation<T, 3>::getAxis() const {
	return axis;
}

template<class T>
inline UnclampedRotation<T, 3> UnclampedRotation<T, 3>::withAxis(const Vector<T, 3>& in_axis) const {
	return UnclampedRotation<T, 3>(in_axis, angle);
}

template<class T>
inline Vector<T, 3> UnclampedRotation<T, 3>::applyTo(const Vector<T, 3>& in_point) const {
	return in_point.rotated(axis, angle);
}

template class UnclampedRotation<float, 2>;
template class UnclampedRotation<double, 2>;
template class UnclampedRotation<float, 3>;
template class UnclampedRotation<double, 3>;
