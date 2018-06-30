#ifndef MATRIX4_H
#define MATRIX4_H

#include "matrix.h"

template<class T>
class Matrix4 : public Matrix<T, 4, 4> {
public:
	using Matrix<T, 4, 4>::Matrix;
	Matrix4(const Matrix<T, 4, 4>& in_source) {
		(*this) = in_source;
	}

	static Matrix4<T> Rotation(const Vector<T, 3>& in_axis, T in_angle) {
		T s = sin(in_angle);
		T c = cos(in_angle);
		T omc = 1 - c;
		T x = in_axis.X();
		T y = in_axis.Y();
		T z = in_axis.Z();
		return {
			 x * x * omc + c, x * y * omc - z * s, x * z * omc + y * s, (T)0,
			 x * y * omc + z * c, y * y * omc + c, y * z * omc - x * s, (T)0,
			 x * z * omc - y * s, y * z * omc + x * s, z * z * omc + c, (T)0,
			 (T)0, (T)0, (T)0, (T)1
		};
	}

	static Matrix4<T> Scale(T in_factor) {
		return {
			in_factor, (T)0, (T)0, (T)0,
			(T)0, in_factor, (T)0, (T)0,
			(T)0, (T)0, in_factor, (T)0,
			(T)0, (T)0, (T)0, (T)1
		};
	}

	static Matrix4<T> Translation(const Vector<T, 3>& in_translation) {
		return {
			(T)1, (T)0, (T)0, in_translation.X(),
			(T)0, (T)1, (T)0, in_translation.Y(),
			(T)0, (T)0, (T)1, in_translation.Z(),
			(T)0, (T)0, (T)0, (T)1
		}
	}
};

using Matrix4f = Matrix4<float>;
using Matrix4d = Matrix4<double>;

#endif
