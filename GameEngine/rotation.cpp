#include "rotation.h"
#include "unclamped_rotation.h"
#include "misc.h"

template<class T>
Rotation<T, 2>::Rotation() :
	angle(0)
{}

template<class T>
Rotation<T, 2>::Rotation(T in_angle) :
	angle(in_angle) {

	Clamp();
}

template<class T>
Rotation<T, 2>::Rotation(T in_from, T in_to) {
	Rotation<T, 2> from(in_from);
	Rotation<T, 2> to(in_to);
	*this = Rotation<T, 2>(from, to);
}

template<class T>
Rotation<T, 2>::Rotation(const Rotation<T, 2>& in_from, const Rotation<T, 2>& in_to) :
	angle(Short_Difference(in_to.angle, in_from.angle)) 
{}

template<class T>
Rotation<T, 2>::Rotation(const UnclampedRotation<T, 2>& in_rotation) :
	angle(in_rotation.Get_Angle())
{}

template<class T>
bool Rotation<T, 2>::Is_Identity() const {
	return angle == 0;
}

template<class T>
void Rotation<T, 2>::Invert() {
	angle = -angle;
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::Get_Inverse() const {
	return Rotation<T, 2>(-angle);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::Followed_By(const Rotation<T, 2>& in_rotation) const {
	return Rotation<T, 2>(angle + in_rotation.angle);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::Lerp(const Rotation<T, 2>& in_rotation, T t) const {
	return Rotation<T, 2>(angle * (1 - t)  + (angle + Short_Difference(in_rotation.angle, angle)) * t);
}

template<class T>
Rotation<T, 2> Rotation<T, 2>::Reflect_Over(const Rotation<T, 2>& in_rotation) const {
	return Lerp(in_rotation, 2);
}

template<class T>
T Rotation<T, 2>::Get_Angle() const {
	return angle;
}

template<class T>
Vector<T, 2> Rotation<T, 2>::Apply_To(const Vector<T, 2>& in_point) const {
	return in_point.Rotated(angle);
}

template<class T>
Matrix<T, 4, 4> Rotation<T, 2>::Get_Matrix() const {
	T s = (T)sin(angle);
	T c = (T)cos(angle);

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
		angle1 -= (T)PI;
	}
	while (angle2 - angle1 > (T)PI) {
		angle2 -= (T)PI;
	}
	return angle2 - angle1;
}

template<class T>
void Rotation<T, 2>::Clamp() {
	while (angle > (T)PI) {
		angle -= (T)PI;
	}
	while (angle < -(T)PI) {
		angle += (T)PI;
	}
}

template<class T>
Rotation<T, 3>::Rotation(const Vector<T, 4>& in_quaternion) :
	quaternion(in_quaternion) 
{}

template<class T>
Rotation<T, 3>::Rotation() :
	quaternion((T)0, (T)0, (T)0, (T)1) 
{}

template<class T>
Rotation<T, 3>::Rotation(const Vector<T, 3>& in_axis, T in_angle) :
	quaternion(in_axis * sin(in_angle / 2), cos(in_angle / 2)) 
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
	quaternion = Vector<T, 4>(axis  * sin(angle / 2), cos(angle / 2));
}

template<class T>
Rotation<T, 3>::Rotation(const Rotation<T, 3>& in_from, const Rotation<T, 3>& in_to) {
	*this = in_from.Get_Inverse().Followed_By(in_to);
}

template<class T>
Rotation<T, 3>::Rotation(const UnclampedRotation<T, 3>& in_rotation) {
	Vector<T, 3> axis = in_rotation.Get_Axis();
	T angle = in_rotation.Get_Angle();
	quaternion = Quaterniond(axis * sin(angle / 2), cos(angle / 2));
}

template<class T>
bool Rotation<T, 3>::Is_Identity() const {
	return quaternion.X() == (T)0 && quaternion.Y() == (T)0 && quaternion.Z() == (T)0 && quaternion.W() == (T)1;
}

template<class T>
void Rotation<T, 3>::Invert() {
	quaternion.Conjugate();
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Get_Inverse() const {
	return Rotation<T, 3>(quaternion.Conjugated());
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Followed_By(const Rotation<T, 3>& in_rotation) const {
	return in_rotation.quaternion.Hamilton(quaternion);
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Lerp(const Rotation<T, 3>& in_rotation, T t) const {
	return (quaternion * (1 - t) + in_rotation.quaternion * t).Normalized();
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Slerp(const Rotation<T, 3>& in_rotation, T t) const {
	T dot = Clamp<T>(quaternion.Dot(in_rotation.quaternion), -1, 1);
	T dotSign = Sign(dot);
	T theta = acos(dot * dotSign);
	return (quaternion * sin((1 - t) * theta) + in_rotation.quaternion * dotSign * sin(t * theta)) / sin(theta);
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::Reflect_Over(const Rotation<T, 3>& in_rotation) const {
	return 2 * quaternion.Dot(in_rotation.quaternion) * in_rotation.quaternion - quaternion;
}

template<class T>
T Rotation<T, 3>::Get_Angle() const {
	return acos(Clamp<T>(quaternion.W(), -1, 1));
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::With_Angle(T in_angle) {
	return Rotation(Get_Axis(), in_angle);
}

template<class T>
Vector<T, 3> Rotation<T, 3>::Get_Axis() const {
	return Vector<T, 3>(quaternion) / sqrt(1 - quaternion.W() * quaternion.W());
}

template<class T>
Rotation<T, 3> Rotation<T, 3>::With_Axis(const Vector<T, 3>& in_axis) const {
	return Rotation(in_axis, Get_Angle());
}

template<class T>
Vector<T, 3> Rotation<T, 3>::Apply_To(const Vector<T, 3>& in_point) const {
	if (!Is_Identity()) {
		return Vector<T, 3>(quaternion.Hamilton(Vector<T, 4>(in_point, 0)).Hamilton(quaternion.Inverse()));
	}
	else {
		return in_point;
	}
}

template<class T>
Matrix<T, 4, 4> Rotation<T, 3>::Get_Matrix() const {
	T x = quaternion.X();
	T y = quaternion.Y();
	T z = quaternion.Z();
	T w = quaternion.W();

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