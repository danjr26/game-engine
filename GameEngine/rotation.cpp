#include "rotation.h"
#include "unclamped_rotation.h"
#include "misc.h"

template<class T>
Rotation<T, 2>::Rotation() :
	mAngle(0)
{}

template<class T>
Rotation<T, 2>::Rotation(T in_angle) :
	mAngle(in_angle) {

	clamp();
}

template<class T>
Rotation<T, 2>::Rotation(const Vector<T, 2>& in_vec) :
	mAngle(in_vec.theta(Vector2d(1, 0)) * ((in_vec.y() < 0) ? -1 : 1))
{}

template<class T>
Rotation<T, 2>::Rotation(T in_from, T in_to) {
	Rotation<T, 2> from(in_from);
	Rotation<T, 2> to(in_to);
	*this = Rotation<T, 2>(from, to);
}

template<class T>
Rotation<T, 2>::Rotation(const Rotation<T, 2>& in_from, const Rotation<T, 2>& in_to) :
	mAngle(shortDifference(in_to.mAngle, in_from.mAngle)) 
{}

template<class T>
Rotation<T, 2>::Rotation(const UnclampedRotation<T, 2>& in_rotation) :
	mAngle(in_rotation.getAngle())
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
Rotation<T, 2> Rotation<T, 2>::followedBy(const Rotation<T, 2>& in_rotation) const {
	return Rotation<T, 2>(mAngle + in_rotation.mAngle);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::lerp(const Rotation<T, 2>& in_rotation, T t) const {
	return Rotation<T, 2>(mAngle * (1 - t)  + (mAngle + shortDifference(in_rotation.mAngle, mAngle)) * t);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::reflectOver(const Rotation<T, 2>& in_rotation) const {
	return lerp(in_rotation, 2);
}

template<class T>
T Rotation<T, 2>::getAngle() const {
	return mAngle;
}

template<class T>
Vector<T, 2> Rotation<T, 2>::applyTo(const Vector<T, 2>& in_point) const {
	return in_point.rotated(mAngle);
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
Rotation<T, 3>::Rotation(const Vector<T, 4>& in_quaternion) :
	mQuaternion(in_quaternion) 
{}

template<class T>
Rotation<T, 3>::Rotation() :
	mQuaternion((T)0, (T)0, (T)0, (T)1) 
{}

template<class T>
Rotation<T, 3>::Rotation(const Vector<T, 3>& in_axis, T in_angle) :
	mQuaternion(in_axis * sin(in_angle / 2), cos(in_angle / 2)) 
{}

template<class T>
Rotation<T, 3>::Rotation(const Vector<T, 3>& in_from, const Vector<T, 3>& in_to) {
	T angle = in_from.theta(in_to);
	Vector<T, 3> axis;
	if (abs(angle - (T)PI) < (T)PI / 10000.0) {
		axis = in_from.cross(Vector<T, 3>(in_from.x(), in_from.y(), in_from.z() + 10).normalized()).normalized();
		angle = (T)PI;
	}
	else {
		axis = in_from.cross(in_to).normalized();
	}
	mQuaternion = Vector<T, 4>(axis  * sin(angle / 2), cos(angle / 2));
}

template<class T>
Rotation<T, 3>::Rotation(const Rotation<T, 3>& in_from, const Rotation<T, 3>& in_to) {
	*this = in_from.getInverse().followedBy(in_to);
}

template<class T>
Rotation<T, 3>::Rotation(const UnclampedRotation<T, 3>& in_rotation) {
	Vector<T, 3> axis = in_rotation.getAxis();
	T angle = in_rotation.getAngle();
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
Rotation<T, 3> Rotation<T, 3>::followedBy(const Rotation<T, 3>& in_rotation) const {
	return in_rotation.mQuaternion.hamilton(mQuaternion);
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::lerp(const Rotation<T, 3>& in_rotation, T t) const {
	return (mQuaternion * (1 - t) + in_rotation.mQuaternion * t).normalized();
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::slerp(const Rotation<T, 3>& in_rotation, T t) const {
	T dot = GEUtil::clamp<T>(mQuaternion.dot(in_rotation.mQuaternion), -1, 1);
	T dotSign = GEUtil::sign(dot);
	T theta = acos(dot * dotSign);
	return (mQuaternion * sin((1 - t) * theta) + in_rotation.mQuaternion * dotSign * sin(t * theta)) / sin(theta);
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::reflectOver(const Rotation<T, 3>& in_rotation) const {
	return 2 * mQuaternion.dot(in_rotation.mQuaternion) * in_rotation.mQuaternion - mQuaternion;
}

template<class T>
T Rotation<T, 3>::getAngle() const {
	return acos(GEUtil::clamp<T>(mQuaternion.w(), -1, 1));
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::withAngle(T in_angle) {
	return Rotation(getAxis(), in_angle);
}

template<class T>
Vector<T, 3> Rotation<T, 3>::getAxis() const {
	return Vector<T, 3>(mQuaternion) / sqrt(1 - mQuaternion.w() * mQuaternion.w());
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::withAxis(const Vector<T, 3>& in_axis) const {
	return Rotation(in_axis, getAngle());
}

template<class T>
Vector<T, 3> Rotation<T, 3>::applyTo(const Vector<T, 3>& in_point) const {
	if (!isIdentity()) {
		return Vector<T, 3>(mQuaternion.hamilton(Vector<T, 4>(in_point, 0)).hamilton(mQuaternion.inverse()));
	}
	else {
		return in_point;
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