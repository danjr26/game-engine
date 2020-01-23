#ifndef ROTATION_H
#define ROTATION_H

#include "../../../Utilities/include/utilities.h"

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
	Rotation(const Rotation<T2, 2>& i_other) : mAngle((T)i_other.angle) {}
	Rotation(T i_angle);
	Rotation(T i_from, T i_to);
	Rotation(const Vector<T, 2>& i_vec);
	Rotation(const Rotation<T, 2>& i_from, const Rotation<T, 2>& i_to);
	Rotation(const UnclampedRotation<T, 2>& i_rotation);

	bool isIdentity() const;
	void invert();
	Rotation<T, 2> getInverse() const;
	Rotation<T, 2> followedBy(const Rotation<T, 2>& i_rotation) const;
	Rotation<T, 2> lerp(const Rotation<T, 2>& i_rotation, T t) const;
	Rotation<T, 2> reflectOver(const Rotation<T, 2>& i_rotation) const;
	T getAngle() const;
	Vector<T, 2> applyTo(const Vector<T, 2>& i_point) const;
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
	Rotation(const Vector<T, 4>& i_quaternion);

public:
	Rotation();
	template<class T2>
	Rotation(const Rotation<T2, 3>& i_other) : mQuaternion(i_other.quaternion) {}
	Rotation(const Vector<T, 3>& i_axis, T i_angle);
	Rotation(const Vector<T, 3>& i_from, const Vector<T, 3>& i_to);
	Rotation(const Rotation<T, 3>& i_from, const Rotation<T, 3>& i_to);
	Rotation(const UnclampedRotation<T, 3>& i_rotation);

	bool isIdentity() const;
	void invert();
	Rotation<T, 3> getInverse() const;
	Rotation<T, 3> followedBy(const Rotation<T, 3>& i_rotation) const;
	Rotation<T, 3> lerp(const Rotation<T, 3>& i_rotation, T i_t) const;
	Rotation<T, 3> slerp(const Rotation<T, 3>& i_rotation, T i_t) const;
	Rotation<T, 3> reflectOver(const Rotation<T, 3>& i_rotation) const;
	T getAngle() const;
	Rotation<T, 3> withAngle(T i_angle);
	Vector<T, 3> getAxis() const;
	Rotation<T, 3> withAxis(const Vector<T, 3>& i_axis) const;
	Vector<T, 3> applyTo(const Vector<T, 3>& i_point) const;
	Matrix<T, 4, 4> getMatrix() const;
};

using Rotation2f = Rotation<float, 2>;
using Rotation2d = Rotation<double, 2>;
using Rotation3f = Rotation<float, 3>;
using Rotation3d = Rotation<double, 3>;

#endif

