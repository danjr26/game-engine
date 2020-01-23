#ifndef UNCLAMPED_ROTATION_H
#define UNCLAMPED_ROTATION_H

#include "../../../Utilities/include/utilities.h"

template<class T, uint n>
class Rotation;

template<class T, uint n>
class UnclampedRotation 
{};

template<class T>
class UnclampedRotation<T, 2> {
	friend class UnclampedRotation;
private:
	T mAngle;

public:
	UnclampedRotation();
	UnclampedRotation(T i_angle);
	UnclampedRotation(T i_from, T i_to);
	UnclampedRotation(const UnclampedRotation<T, 2>& i_from, const UnclampedRotation<T, 2>& i_to);
	UnclampedRotation(const Rotation<T, 2>& i_rotation);

	bool isIdentity() const;
	void invert();
	UnclampedRotation<T, 2> getInverse() const;
	UnclampedRotation<T, 2> operator+(const UnclampedRotation<T, 2>& i_rotation) const;
	UnclampedRotation<T, 2> operator-(const UnclampedRotation<T, 2>& i_rotation) const;
	UnclampedRotation<T, 2> operator*(T i_scalar) const;
	UnclampedRotation<T, 2> operator/(T i_scalar) const;
	void operator+=(const UnclampedRotation<T, 2>& i_rotation);
	void operator-=(const UnclampedRotation<T, 2>& i_rotation);
	void operator*=(T i_scalar);
	void operator/=(T i_scalar);
	UnclampedRotation<T, 2> lerp(const UnclampedRotation<T, 2>& i_rotation, T t) const;
	T getAngle() const;
	Vector<T, 2> applyTo(const Vector<T, 2>& i_point) const;
};

template<class T>
class UnclampedRotation<T, 3> {
	friend class UnclampedRotation;
private:
	Vector<T, 3> axis;
	T angle; // radians

public:
	UnclampedRotation();
	UnclampedRotation(const Vector<T, 3>& i_axis, T i_angle);
	UnclampedRotation(const Vector<T, 3>& i_from, const Vector<T, 3>& i_to);
	UnclampedRotation(const Rotation<T, 3>& i_rotation);

	bool isIdentity() const;
	void invert();
	UnclampedRotation<T, 3> getInverse() const;
	UnclampedRotation<T, 3> operator+(const UnclampedRotation<T, 3>& i_rotation) const;
	UnclampedRotation<T, 3> operator-(const UnclampedRotation<T, 3>& i_rotation) const;
	UnclampedRotation<T, 3> operator*(T i_scalar) const;
	UnclampedRotation<T, 3> operator/(T i_scalar) const;
	void operator+=(const UnclampedRotation<T, 3>& i_rotation);
	void operator-=(const UnclampedRotation<T, 3>& i_rotation);
	void operator*=(T i_scalar);
	void operator/=(T i_scalar);
	UnclampedRotation<T, 3> lerp(const UnclampedRotation<T, 3>& i_rotation, T t) const;
	T getAngle() const;
	UnclampedRotation<T, 3> withAngle(T i_angle) const;
	Vector<T, 3> getAxis() const;
	UnclampedRotation<T, 3> withAxis(const Vector<T, 3>& i_axis) const;
	Vector<T, 3> applyTo(const Vector<T, 3>& i_point) const;
};

template<class T, uint n>
using URotation = UnclampedRotation<T, n>;

using URotation2f = UnclampedRotation<float, 2>;
using URotation2d = UnclampedRotation<double, 2>;
using UnclampedRotation2f = UnclampedRotation<float, 2>;
using UnclampedRotation2d = UnclampedRotation<double, 2>;

using URotation3f = UnclampedRotation<float, 3>;
using URotation3d = UnclampedRotation<double, 3>;
using UnclampedRotation3f = UnclampedRotation<float, 3>;
using UnclampedRotation3d = UnclampedRotation<double, 3>;

#endif




