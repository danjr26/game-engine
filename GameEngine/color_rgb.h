#ifndef COLOR_RGB_H
#define COLOR_RGB_H

#include "vector.h"

template<class T>
class ColorRGB : public Vector<T, 3> {
public:
	using Vector<T, 3>::Vector;

	T R() const {
		return members[0];
	}

	T G() const {
		return members[1];
	}

	T B() const {
		return members[2];
	}

	T Dot(const Vector<T, 3>& v) const = delete;
	T Dot_Self() const = delete;
	T Magnitude() const = delete;
	Vector<T, 3> Normalized() const = delete;
	void Normalize() = delete;
	T Theta(const Vector<T, 3>& v) const = delete;
};

using ColorRGBc = ColorRGB<uchar>;
using ColorRGBf = ColorRGB<float>;

#endif