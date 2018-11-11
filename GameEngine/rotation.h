#ifndef ROTATION_H
#define ROTATION_H

#include "vector.h"
#include "matrix.h"

template<class T, uint n>
class UnclampedRotation;

template<class T, uint n>
class Rotation
{};

template<class T>
class Rotation<T, 2> {
private:
	T angle;

public:
	Rotation();
	Rotation(T in_angle);
	Rotation(T in_from, T in_to);
	Rotation(const Rotation<T, 2>& in_from, const Rotation<T, 2>& in_to);
	Rotation(const UnclampedRotation<T, 2>& in_rotation);

	bool Is_Identity() const;
	void Invert();
	Rotation<T, 2> Get_Inverse() const;
	Rotation<T, 2> Followed_By(const Rotation<T, 2>& in_rotation) const;
	Rotation<T, 2> Lerp(const Rotation<T, 2>& in_rotation, T t) const;
	Rotation<T, 2> Reflect_Over(const Rotation<T, 2>& in_rotation) const;
	T Get_Angle() const;
	Vector<T, 2> Apply_To(const Vector<T, 2>& in_point) const;
	Matrix<T, 4, 4> Get_Matrix() const;

private:
	void Clamp();

	static T Short_Difference(T angle1, T angle2);
};

template<class T>
class Rotation<T, 3> {
private:
	Vector<T, 4> quaternion;

private:
	Rotation(const Vector<T, 4>& in_quaternion);

public:
	Rotation();
	Rotation(const Vector<T, 3>& in_axis, T in_angle);
	Rotation(const Vector<T, 3>& in_from, const Vector<T, 3>& in_to);
	Rotation(const Rotation<T, 3>& in_from, const Rotation<T, 3>& in_to);
	Rotation(const UnclampedRotation<T, 3>& in_rotation);

	bool Is_Identity() const;
	void Invert();
	Rotation<T, 3> Get_Inverse() const;
	Rotation<T, 3> Followed_By(const Rotation<T, 3>& in_rotation) const;
	Rotation<T, 3> Lerp(const Rotation<T, 3>& in_rotation, T t) const;
	Rotation<T, 3> Slerp(const Rotation<T, 3>& in_rotation, T t) const;
	Rotation<T, 3> Reflect_Over(const Rotation<T, 3>& in_rotation) const;
	T Get_Angle() const;
	Rotation<T, 3> With_Angle(T in_angle);
	Vector<T, 3> Get_Axis() const;
	Rotation<T, 3> With_Axis(const Vector<T, 3>& in_axis) const;
	Vector<T, 3> Apply_To(const Vector<T, 3>& in_point) const;
	Matrix<T, 4, 4> Get_Matrix() const;
};

using Rotation2f = Rotation<float, 2>;
using Rotation2d = Rotation<double, 2>;
using Rotation3f = Rotation<float, 3>;
using Rotation3d = Rotation<double, 3>;

#endif

