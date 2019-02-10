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
	friend class Rotation;
private:
	T mAngle;

public:
	Rotation();
	template<class T2>
	Rotation(const Rotation<T2, 2>& in_other) : mAngle((T)in_other.angle) {}
	Rotation(T in_angle);
	Rotation(T in_from, T in_to);
	Rotation(const Vector<T, 2>& in_vec);
	Rotation(const Rotation<T, 2>& in_from, const Rotation<T, 2>& in_to);
	Rotation(const UnclampedRotation<T, 2>& in_rotation);

	bool isIdentity() const;
	void invert();
	Rotation<T, 2> getInverse() const;
	Rotation<T, 2> followedBy(const Rotation<T, 2>& in_rotation) const;
	Rotation<T, 2> lerp(const Rotation<T, 2>& in_rotation, T t) const;
	Rotation<T, 2> reflectOver(const Rotation<T, 2>& in_rotation) const;
	T getAngle() const;
	Vector<T, 2> applyTo(const Vector<T, 2>& in_point) const;
	Matrix<T, 4, 4> getMatrix() const;

private:
	void clamp();

	static T shortDifference(T angle1, T angle2);
};

template<class T>
class Rotation<T, 3> {
	friend class Rotation;
private:
	Vector<T, 4> mQuaternion;

private:
	Rotation(const Vector<T, 4>& in_quaternion);

public:
	Rotation();
	template<class T2>
	Rotation(const Rotation<T2, 3>& in_other) : mQuaternion(in_other.quaternion) {}
	Rotation(const Vector<T, 3>& in_axis, T in_angle);
	Rotation(const Vector<T, 3>& in_from, const Vector<T, 3>& in_to);
	Rotation(const Rotation<T, 3>& in_from, const Rotation<T, 3>& in_to);
	Rotation(const UnclampedRotation<T, 3>& in_rotation);

	bool isIdentity() const;
	void invert();
	Rotation<T, 3> getInverse() const;
	Rotation<T, 3> followedBy(const Rotation<T, 3>& in_rotation) const;
	Rotation<T, 3> lerp(const Rotation<T, 3>& in_rotation, T t) const;
	Rotation<T, 3> slerp(const Rotation<T, 3>& in_rotation, T t) const;
	Rotation<T, 3> reflectOver(const Rotation<T, 3>& in_rotation) const;
	T getAngle() const;
	Rotation<T, 3> withAngle(T in_angle);
	Vector<T, 3> getAxis() const;
	Rotation<T, 3> withAxis(const Vector<T, 3>& in_axis) const;
	Vector<T, 3> applyTo(const Vector<T, 3>& in_point) const;
	Matrix<T, 4, 4> getMatrix() const;
};

using Rotation2f = Rotation<float, 2>;
using Rotation2d = Rotation<double, 2>;
using Rotation3f = Rotation<float, 3>;
using Rotation3d = Rotation<double, 3>;

#endif

