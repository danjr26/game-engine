#ifndef COLOR_IA_H
#define COLOR_IA_H

#include "vector.h"

template<class T>
class ColorIA : public Vector<T, 2> {
public:
	using Vector<T, 2>::Vector;

	T I() const {
		return members[0];
	}

	T A() const {
		return members[1];
	}

	T Dot(const Vector<T, 2>& v) const = delete;
	T Dot_Self() const = delete;
	T Magnitude() const = delete;
	Vector<T, 2> Normalized() const = delete;
	void Normalize() = delete;
	T Theta(const Vector<T, 2>& v) const = delete;
};

using ColorIAc = ColorIA<uchar>;
using ColorIAf = ColorIA<float>;


#endif