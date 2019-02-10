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

	Clamp();
}

template<class T>
Rotation<T, 2>::Rotation(const Vector<T, 2>& in_vec) :
	mAngle(in_vec.Theta(Vector2d(1, 0)))
{}

template<class T>
Rotation<T, 2>::Rotation(T in_from, T in_to) {
	Rotation<T, 2> from(in_from);
	Rotation<T, 2> to(in_to);
	*this = Rotation<T, 2>(from, to);
}

template<class T>
Rotation<T, 2>::Rotation(const Rotation<T, 2>& in_from, const Rotation<T, 2>& in_to) :
	mAngle(Short_Difference(in_to.mAngle, in_from.mAngle)) 
{}

template<class T>
Rotation<T, 2>::Rotation(const UnclampedRotation<T, 2>& in_rotation) :
	mAngle(in_rotation.Get_Angle())
{}

template<class T>
bool Rotation<T, 2>::Is_Identity() const {
	return mAngle == 0;
}

template<class T>
void Rotation<T, 2>::Invert() {
	mAngle = -mAngle;
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::Get_Inverse() const {
	return Rotation<T, 2>(-mAngle);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::Followed_By(const Rotation<T, 2>& in_rotation) const {
	return Rotation<T, 2>(mAngle + in_rotation.mAngle);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::Lerp(const Rotation<T, 2>& in_rotation, T t) const {
	return Rotation<T, 2>(mAngle * (1 - t)  + (mAngle + Short_Difference(in_rotation.mAngle, mAngle)) * t);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::Reflect_Over(const Rotation<T, 2>& in_rotation) const {
	return Lerp(in_rotation, 2);
}

template<class T>
T Rotation<T, 2>::Get_Angle() const {
	return mAngle;
}

template<class T>
Vector<T, 2> Rotation<T, 2>::Apply_To(const Vector<T, 2>& in_point) const {
	return in_point.Rotated(mAngle);
}

template<class T>
Matrix<T, 4, 4> Rotation<T, 2>::Get_Matrix() const {
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
T Rotation<T, 2>::Short_Difference(T angle1, T angle2) {
	while (angle1 - angle2 > (T)PI) {
		angle1 -= (T)(2.0 * PI);
	}
	while (angle2 - angle1 > (T)PI) {
		angle2 -= (T)(2.0 * PI);
	}
	return angle2 - angle1;
}

template<class T>
void Rotation<T, 2>::Clamp() {
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
	T angle = in_from.Theta(in_to);
	Vector<T, 3> axis;
	if (abs(angle - (T)PI) < (T)PI / 10000.0) {
		axis = in_from.Cross(Vector<T, 3>(in_from.X(), in_from.Y(), in_from.Z() + 10).Normalized()).Normalized();
		angle = (T)PI;
	}
	else {
		axis = in_from.Cross(in_to).Normalized();
	}
	mQuaternion = Vector<T, 4>(axis  * sin(angle / 2), cos(angle / 2));
}

template<class T>
Rotation<T, 3>::Rotation(const Rotation<T, 3>& in_from, const Rotation<T, 3>& in_to) {
	*this = in_from.Get_Inverse().Followed_By(in_to);
}

template<class T>
Rotation<T, 3>::Rotation(const UnclampedRotation<T, 3>& in_rotation) {
	Vector<T, 3> axis = in_rotation.Get_Axis();
	T angle = in_rotation.Get_Angle();
	mQuaternion = Quaterniond(axis * sin(angle / 2), cos(angle / 2));
}

template<class T>
bool Rotation<T, 3>::Is_Identity() const {
	return mQuaternion.X() == (T)0 && mQuaternion.Y() == (T)0 && mQuaternion.Z() == (T)0 && mQuaternion.W() == (T)1;
}

template<class T>
void Rotation<T, 3>::Invert() {
	mQuaternion.Conjugate();
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Get_Inverse() const {
	return Rotation<T, 3>(mQuaternion.Conjugated());
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Followed_By(const Rotation<T, 3>& in_rotation) const {
	return in_rotation.mQuaternion.Hamilton(mQuaternion);
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Lerp(const Rotation<T, 3>& in_rotation, T t) const {
	return (mQuaternion * (1 - t) + in_rotation.mQuaternion * t).Normalized();
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Slerp(const Rotation<T, 3>& in_rotation, T t) const {
	T dot = Clamp<T>(mQuaternion.Dot(in_rotation.mQuaternion), -1, 1);
	T dotSign = Sign(dot);
	T theta = acos(dot * dotSign);
	return (mQuaternion * sin((1 - t) * theta) + in_rotation.mQuaternion * dotSign * sin(t * theta)) / sin(theta);
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Reflect_Over(const Rotation<T, 3>& in_rotation) const {
	return 2 * mQuaternion.Dot(in_rotation.mQuaternion) * in_rotation.mQuaternion - mQuaternion;
}

template<class T>
T Rotation<T, 3>::Get_Angle() const {
	return acos(Clamp<T>(mQuaternion.W(), -1, 1));
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::With_Angle(T in_angle) {
	return Rotation(Get_Axis(), in_angle);
}

template<class T>
Vector<T, 3> Rotation<T, 3>::Get_Axis() const {
	return Vector<T, 3>(mQuaternion) / sqrt(1 - mQuaternion.W() * mQuaternion.W());
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::With_Axis(const Vector<T, 3>& in_axis) const {
	return Rotation(in_axis, Get_Angle());
}

template<class T>
Vector<T, 3> Rotation<T, 3>::Apply_To(const Vector<T, 3>& in_point) const {
	if (!Is_Identity()) {
		return Vector<T, 3>(mQuaternion.Hamilton(Vector<T, 4>(in_point, 0)).Hamilton(mQuaternion.Inverse()));
	}
	else {
		return in_point;
	}
}

template<class T>
Matrix<T, 4, 4> Rotation<T, 3>::Get_Matrix() const {
	T x = mQuaternion.X();
	T y = mQuaternion.Y();
	T z = mQuaternion.Z();
	T w = mQuaternion.W();

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