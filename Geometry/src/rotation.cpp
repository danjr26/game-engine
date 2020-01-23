#include "../include/internal/rotation.h"
#include "../include/internal/unclamped_rotation.h"

template<class T>
Rotation<T, 2>::Rotation() :
	mAngle(0)
{}

template<class T>
Rotation<T, 2>::Rotation(T i_angle) :
	mAngle(i_angle) {

	clamp();
}

template<class T>
Rotation<T, 2>::Rotation(const Vector<T, 2>& i_vec) :
	mAngle(i_vec.theta(Vector2d(1, 0)) * ((i_vec.y() < 0) ? -1 : 1))
{}

template<class T>
Rotation<T, 2>::Rotation(T i_from, T i_to) {
	Rotation<T, 2> from(i_from);
	Rotation<T, 2> to(i_to);
	*this = Rotation<T, 2>(from, to);
}

template<class T>
Rotation<T, 2>::Rotation(const Rotation<T, 2>& i_from, const Rotation<T, 2>& i_to) :
	mAngle(shortDifference(i_to.mAngle, i_from.mAngle)) 
{}

template<class T>
Rotation<T, 2>::Rotation(const UnclampedRotation<T, 2>& i_rotation) :
	mAngle(i_rotation.getAngle())
{}

template<class T>
bool Rotation<T, 2>::isIdentity() const {
	return mAngle == 0;
}

template<class T>
void Rotation<T, 2>::invert() {
	mAngle = -mAngle;
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::getInverse() const {
	return Rotation<T, 2>(-mAngle);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::followedBy(const Rotation<T, 2>& i_rotation) const {
	return Rotation<T, 2>(mAngle + i_rotation.mAngle);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::lerp(const Rotation<T, 2>& i_rotation, T t) const {
	return Rotation<T, 2>(mAngle * (1 - t)  + (mAngle + shortDifference(i_rotation.mAngle, mAngle)) * t);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::reflectOver(const Rotation<T, 2>& i_rotation) const {
	return lerp(i_rotation, 2);
}

template<class T>
T Rotation<T, 2>::getAngle() const {
	return mAngle;
}

template<class T>
Vector<T, 2> Rotation<T, 2>::applyTo(const Vector<T, 2>& i_point) const {
	return i_point.rotated(mAngle);
}

template<class T>
Matrix<T, 4, 4> Rotation<T, 2>::getMatrix() const {
	T s = (T)sin(mAngle);
	T c = (T)cos(mAngle);

	return {
		c, -s, (T)0, (T)0,
		s, c, (T)0, (T)0,
		(T)0, (T)0, (T)1, (T)0,
		(T)0, (T)0, (T)0, (T)1
	};
}

template<class T>
T Rotation<T, 2>::shortDifference(T angle1, T angle2) {
	while (angle1 - angle2 > (T)PI) {
		angle1 -= (T)(2.0 * PI);
	}
	while (angle2 - angle1 > (T)PI) {
		angle2 -= (T)(2.0 * PI);
	}
	return angle2 - angle1;
}

template<class T>
void Rotation<T, 2>::clamp() {
	while (mAngle > (T)PI) {
		mAngle -= (T)(2.0 * PI);
	}
	while (mAngle < -(T)PI) {
		mAngle += (T)(2.0 * PI);
	}
}

template<class T>
Rotation<T, 3>::Rotation(const Vector<T, 4>& i_quaternion) :
	mQuaternion(i_quaternion) 
{}

template<class T>
Rotation<T, 3>::Rotation() :
	mQuaternion((T)0, (T)0, (T)0, (T)1) 
{}

template<class T>
Rotation<T, 3>::Rotation(const Vector<T, 3>& i_axis, T i_angle) :
	mQuaternion(i_axis * sin(i_angle / 2), cos(i_angle / 2)) 
{}

template<class T>
Rotation<T, 3>::Rotation(const Vector<T, 3>& i_from, const Vector<T, 3>& i_to) {
	T angle = i_from.theta(i_to);
	Vector<T, 3> axis;
	if (abs(angle - (T)PI) < (T)PI / 10000.0) {
		axis = i_from.cross(Vector<T, 3>(i_from.x(), i_from.y(), i_from.z() + 10).normalized()).normalized();
		angle = (T)PI;
	}
	else {
		axis = i_from.cross(i_to).normalized();
	}
	mQuaternion = Vector<T, 4>(axis  * sin(angle / 2), cos(angle / 2));
}

template<class T>
Rotation<T, 3>::Rotation(const Rotation<T, 3>& i_from, const Rotation<T, 3>& i_to) {
	*this = i_from.getInverse().followedBy(i_to);
}

template<class T>
Rotation<T, 3>::Rotation(const UnclampedRotation<T, 3>& i_rotation) {
	Vector<T, 3> axis = i_rotation.getAxis();
	T angle = i_rotation.getAngle();
	mQuaternion = Quaterniond(axis * sin(angle / 2), cos(angle / 2));
}

template<class T>
bool Rotation<T, 3>::isIdentity() const {
	return mQuaternion.x() == (T)0 && mQuaternion.y() == (T)0 && mQuaternion.z() == (T)0 && mQuaternion.w() == (T)1;
}

template<class T>
void Rotation<T, 3>::invert() {
	mQuaternion.conjugate();
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::getInverse() const {
	return Rotation<T, 3>(mQuaternion.conjugated());
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::followedBy(const Rotation<T, 3>& i_rotation) const {
	return i_rotation.mQuaternion.hamilton(mQuaternion);
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::lerp(const Rotation<T, 3>& i_rotation, T t) const {
	return (mQuaternion * (1 - t) + i_rotation.mQuaternion * t).normalized();
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::slerp(const Rotation<T, 3>& i_rotation, T t) const {
	T dot = clamp<T>(mQuaternion.dot(i_rotation.mQuaternion), -1, 1);
	T dotSign = sign(dot);
	T theta = acos(dot * dotSign);
	return (mQuaternion * sin((1 - t) * theta) + i_rotation.mQuaternion * dotSign * sin(t * theta)) / sin(theta);
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::reflectOver(const Rotation<T, 3>& i_rotation) const {
	return 2 * mQuaternion.dot(i_rotation.mQuaternion) * i_rotation.mQuaternion - mQuaternion;
}

template<class T>
T Rotation<T, 3>::getAngle() const {
	return acos(clamp<T>(mQuaternion.w(), -1, 1));
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::withAngle(T i_angle) {
	return Rotation(getAxis(), i_angle);
}

template<class T>
Vector<T, 3> Rotation<T, 3>::getAxis() const {
	return Vector<T, 3>(mQuaternion) / sqrt(1 - mQuaternion.w() * mQuaternion.w());
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::withAxis(const Vector<T, 3>& i_axis) const {
	return Rotation(i_axis, getAngle());
}

template<class T>
Vector<T, 3> Rotation<T, 3>::applyTo(const Vector<T, 3>& i_point) const {
	if (!isIdentity()) {
		return Vector<T, 3>(mQuaternion.hamilton(Vector<T, 4>(i_point, 0)).hamilton(mQuaternion.inverse()));
	}
	else {
		return i_point;
	}
}

template<class T>
Matrix<T, 4, 4> Rotation<T, 3>::getMatrix() const {
	T x = mQuaternion.x();
	T y = mQuaternion.y();
	T z = mQuaternion.z();
	T w = mQuaternion.w();

	T xx = 2 * x * x;
	T xy = 2 * x * y;
	T xz = 2 * x * z;
	T xw = 2 * x * w;
	T yy = 2 * y * y;
	T yz = 2 * y * z;
	T yw = 2 * y * w;
	T zz = 2 * z * z;
	T zw = 2 * z * w;
	T ww = 2 * w * w;

	return {
		1 - yy - zz, xy - zw, xz - yw, (T)0,
		xy + zw, 1 - zz - xx, yz - xw, (T)0,
		xz - yw, yz + xw, 1 - xx - yy, (T)0,
		(T)0, (T)0, (T)0, (T)1
	};
}

template class Rotation<float, 2>;
template class Rotation<double, 2>;
template class Rotation<float, 3>;
template class Rotation<double, 3>;