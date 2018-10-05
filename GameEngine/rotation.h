#ifndef ROTATION_H
#define ROTATION_H

#include "vector.h"
#include "matrix.h"

template<class T>
class Rotation {
private:
	Vector<T, 4> quaternion;

private:
	Rotation(const Vector<T, 4>& in_quaternion) :
		quaternion(in_quaternion)
	{}

public:
	Rotation() :
		quaternion(0, 0, 0, 1)
	{}

	Rotation(const Vector<T, 3>& in_axis, T in_angle) :
		quaternion(in_axis * sin(in_angle / 2), cos(in_angle / 2))
	{}

	Rotation(const Vector<T, 3>& in_from, const Vector<T, 3> in_to) {
		T angle = in_from.Theta(in_to);
		Vector<T, 3> axis;
		if (abs(angle - PI) < PI / 10000.0) {
			axis = in_from.Cross(Vector<T, 3>(in_from.X(), in_from.Y(), in_from.Z() + 10).Normalized()).Normalized();
			angle = PI;
		}
		else {
			axis = in_from.Cross(in_to).Normalized();
		}
		quaternion = Vector<T, 4>(axis  * sin(angle / 2), cos(angle / 2));
	}

	bool Is_Identity() const {
		return quaternion == Vector<T, 4>(0, 0, 0, 1);
	}

	void Invert() {
		quaternion.Conjugate();
	}

	Rotation<T> Get_Inverse() const {
		return Rotation<T>(quaternion.Conjugated());
	}

	Rotation<T> Followed_By(const Rotation<T>& in_rotation) const {
		return in_rotation.quaternion.Hamilton(quaternion);
	}

	Rotation<T> Lerp(const Rotation<T>& in_rotation, T t) const {
		return (quaternion * (1 - t) + in_rotation.quaternion * t).Normalized();
	}

	Rotation<T> Slerp(const Rotation<T>& in_rotation, T t) const {
		T dot = Clamp<T>(quaternion.Dot(in_rotation.quaternion), -1, 1);
		T dotSign = Sign(dot);
		T theta = acos(dot * dotSign);
		return (sin((1 - t) * theta) * quaternion + dotSign * sin(t * theta) * in_rotation.quaternion) / sin(theta);
	}

	Rotation<T> Reflect_Over(const Rotation<T>& in_rotation) const {
		return 2 * quaternion.Dot(in_rotation.quaternion) * in_rotation.quaternion - quaternion;
	}

	T Get_Angle() const {
		return acos(Clamp<T>(members[3], -1, 1));
	}

	Rotation<T> With_Angle(T in_angle) {
		return Rotation(in_angle, Get_Axis());
	}

	Vector<T, 3> Get_Axis() const {
		return Vector<T, 3>(quaternion) / sqrt(1 - members[3] * members[3]);
	}

	Rotation<T> With_Axis(const Vector<T, 3>& in_axis) const {
		return Rotation(Get_Angle(), in_axis);
	}

	Vector<T, 3> Apply_To(const Vector<T, 3>& in_point) const {
		return Vector<T, 3>(quaternion.Hamilton(Vector<T, 4>(in_point, 0)).Hamilton(quaternion.Inverse()));
	}

	Matrix<T, 4, 4> Get_Matrix() const {
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
};

using Rotationf = Rotation<float>;
using Rotationd = Rotation<double>;

#endif