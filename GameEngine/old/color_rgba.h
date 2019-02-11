#ifndef COLOR_RGBA_H
#define COLOR_RGBA_H

#include "vector.h"

template<class T>
class ColorRGBA : public Vector<T, 4> {
public:
	using Vector<T, 4>::Vector;
	
	T R() const {
		return members[0];
	}

	T G() const {
		return members[1];
	}

	T B() const {
		return members[2];
	}

	T A() const {
		return members[3];
	}

	T Dot(const Vector<T, 4>& v) const = delete;
	T Dot_Self() const = delete;
	T Magnitude() const = delete;
	Vector<T, 4> Normalized() const = delete;
	void Normalize() = delete;
	T Theta(const Vector<T, 4>& v) const = delete;
};

using ColorRGBAc = ColorRGBA<uchar>;
using ColorRGBAf = ColorRGBA<float>;

#endif