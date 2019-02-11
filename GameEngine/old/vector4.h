#ifndef VECTOR4_H
#define VECTOR4_H

#include "vector.h"

template<class T>
class Vector4 : public Vector<T, 4> {
	using Vector<T, 4>::Vector;

	T X() {
		return members[0];
	}

	T Y() {
		return members[1];
	}

	T Z() {
		return members[2];
	}

	T W() {
		return members[3];
	}
};

using Vector4i = Vector4<int>;
using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;

#endif