#ifndef MATRIX_H
#define MATRIX_H

#include <sstream>
#include <iomanip>
#include "definitions.h"
#include "vector.h"

template<class T, uint m, uint n>
class Matrix {
	friend class Matrix;
protected:
	T data[m][n];

public:
	Matrix() 
	{}

	Matrix(T in_data[m][n]) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] = in_data[i][j];
			}
		}
	}

	template<typename... Args, typename = typename std::enable_if<sizeof...(Args) == m * n, void>::type>
	Matrix(Args... in_data) :
		data{ in_data... }
	{}

	Matrix(const T* in_data) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] = in_data[i * n + j];
			}
		}
	}

	Matrix(const T in_data[m][n]) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] = in_data[i][j];
			}
		}
	}

	template<typename = typename std::enable_if<n == 1, void>::type>
	Matrix(const Vector<T, m>& vec) {
		for (uint i = 0; i < m; i++) {
			data[i][0] = vec.Get(i);
		}
	}

	Matrix(const Matrix<T, m, n>& mat) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] = mat.data[i][j];
			}
		}
	}

	template<class T2>
	Matrix(const Matrix<T2, m, n>& mat) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] = (T)mat.data[i][j];
			}
		}
	}

	template<typename = typename std::enable_if<n == 1, void>::type>
	explicit operator Vector<T, m> () {
		Vector<T, m> vOut = Vector<T, m>();
		for (uint i = 0; i < m; i++) {
			vOut[i] = data[i][0];
		}
		return vOut;
	}

	T Element(uint i, uint j) const {
		return data[i][j];
	}

	Vector<T, n> Row(uint i) const {
		return Vector<T, n>(&data[i][0]);
	}

	Vector<T, m> Column(uint j) const {
		Vector<T, n> vOut();
		for (uint i = 0; i < m; i++) {
			vOut[i] = data[i][j];
		}
	} 

	bool Element_Is_Zero(uint i, uint j) const {
		return data[i][j] == 0;
	}

	bool Row_Is_Zero(uint i) {
		for (uint j = 0; j < n; j++) {
			if (data[i][j] != 0) {
				return false;
			}
		}
		return true;
	}

	bool Column_Is_Zero(uint j) const {
		for (uint i = 0; i < m; i++) {
			if (data[i][j] != 0) {
				return false;
			}
		}
		return true;
	}

	bool Submatrix_Is_Zero(uint i1, uint j1, uint i2, uint j2) const {
		for (uint i = i1; i <= i2; i++) {
			for (uint j = j1; j < j2; j++) {
				if (data[i][j] != 0) {
					return false;
				}
			}
		}
		return true;
	}

	void Element(uint i, uint j, T value) {
		data[i][j] = value;
	}

	void Row(uint i, const Vector<T, n>& values) {
		for (uint j = 0; j < n; j++) {
			data[i][j] = values[j];
		}
	}
	
	void Column(uint j, const Vector<T, m>& values) {
		for (uint i = 0; i < m; i++) {
			data[i][j] = values.Get(i);
		}
	}

	Matrix<T, m, n> operator+(const Matrix<T, m, n>& mat) const {
		Matrix<T, m, n> matOut = data;
		matOut += mat;
		return matOut;
	}

	Matrix<T, m, n> operator-(const Matrix<T, m, n>& mat) const {
		Matrix<T, m, n> matOut = data;
		matOut -= mat;
		return matOut;
	}

	template<uint p>
	Matrix<T, m, p> operator*(const Matrix<T, n, p>& mat) const {
		Matrix<T, m, p> matOut = Matrix<T, m, p>();
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < p; j++) {
				matOut.data[i][j] = 0;
				for (uint k = 0; k < n; k++) {
					matOut.data[i][j] += data[i][k] * mat.data[k][j];
				}
			}
		}
		return matOut;
	}

	Vector<T, m> operator*(const Vector<T, n>& vec) const {
		return Vector<T, m>((*this) * Matrix<T, n, 1>(vec));
	}

	Matrix<T, m, n> operator*(T scalar) const {
		Matrix<T, m, n> matOut = data;
		matOut *= scalar;
		return matOut;
	}

	Matrix<T, m, n> operator/(T scalar) const {
		Matrix<T, m, n> matOut = data;
		matOut /= scalar;
		return matOut;
	}

	void operator+=(const Matrix<T, m, n>& mat) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] += mat.data[i][j];
			}
		}
	}

	void operator-=(const Matrix<T, m, n>& mat) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] -= mat.data[i][j];
			}
		}
	}
	
	template<typename = typename std::enable_if<m == n, void>::type>
	void operator *=(const Matrix<T, m, n>& mat) {
		(*this) = Matrix<T, m, n>(data) * mat;
	}

	void operator*=(T scalar) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] *= scalar;
			}
		}
	}

	void operator/=(T scalar) {
		Matrix<T, m, n> matOut = Matrix<T, m, n>();
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				data[i][j] *= scalar;
			}
		}
	}

	void Transpose() {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				if (i != j) {
					std::swap(data[i][j], data[j][i]);
				}
			}
		}
	}

	static Matrix<T, m, n> Identity() {
		Matrix<T, m, n> matOut = Matrix<T, m, n>();
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				if (i == j) {
					matOut.data[i][j] = 1;
				}
				else {
					matOut.data[i][j] = 0;
				}
			}
		}
		return matOut;
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> Rotation(const Vector<T, 3>& in_axis, T in_angle) {
		T s = sin(in_angle);
		T c = cos(in_angle);
		T omc = 1 - c;
		T x = in_axis.X();
		T y = in_axis.Y();
		T z = in_axis.Z();
		return {
			x * x * omc + c, x * y * omc - z * s, x * z * omc + y * s, (T)0,
			x * y * omc + z * s, y * y * omc + c, y * z * omc - x * s, (T)0,
			x * z * omc - y * s, y * z * omc + x * s, z * z * omc + c, (T)0,
			(T)0, (T)0, (T)0, (T)1
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> Scale(const Vector<T, 3>& in_factor) {
		return {
			in_factor.X(), (T)0, (T)0, (T)0,
			(T)0, in_factor.Y(), (T)0, (T)0,
			(T)0, (T)0, in_factor.Z(), (T)0,
			(T)0, (T)0, (T)0, (T)1
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> Translation(const Vector<T, 3>& in_translation) {
		return {
			(T)1, (T)0, (T)0, in_translation.X(),
			(T)0, (T)1, (T)0, in_translation.Y(),
			(T)0, (T)0, (T)1, in_translation.Z(),
			(T)0, (T)0, (T)0, (T)1
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> Orthographic(Vector3f in_minima, Vector3f in_maxima) {
		return {
			2.0f / (in_maxima[0] - in_minima[0]), 0.0f, 0.0f, -(in_maxima[0] + in_minima[0]) / (in_maxima[0] - in_minima[0]),
			0.0f, 2.0f / (in_maxima[1] - in_minima[1]), 0.0f, -(in_maxima[1] + in_minima[1]) / (in_maxima[1] - in_minima[1]),
			0.0f, 0.0f, 2.0f / (in_maxima[2] - in_minima[2]), -(in_maxima[2] + in_minima[2]) / (in_maxima[2] - in_minima[2]),
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> Perspective(float in_near, float in_far, float in_horizontalAngle, float aspectRatio) {
		float right = tanf(in_horizontalAngle) * in_near;
		float left = -right;
		float top = right / aspectRatio;
		float bottom = -top;
		return {
			2.0f * in_near / (right - left), 0, (right + left) / (right - left), 0.0f,
			0.0f, 2.0f * in_near / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
			0.0f, 0.0f, -(in_far + in_near) / (in_far - in_near), -(2 * in_far * in_near) / (in_far - in_near),
			0.0f, 0.0f, -1.0f, 0.0f
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> Perspective(Vector3f in_minima, Vector3f in_maxima) {
		return {
			2.0f * in_minima[2] / (in_maxima[0] - in_minima[0]), 0.0f, (in_maxima[0] + in_minima[0]) / (in_maxima[0] - in_minima[0]), 0.0f,
			0.0f, 2.0f * in_minima[2] / (in_maxima[1] - in_minima[1]), (in_maxima[1] + in_minima[1]) / (in_maxima[1] - in_minima[1]), 0.0f,
			0.0f, 0.0f, -(in_maxima[2] + in_minima[2]) / (in_maxima[2] - in_minima[2]), -(2 * in_maxima[2] * in_minima[2]) / (in_maxima[2] - in_minima[2]),
			0.0f, 0.0f, -1.0f, 0.0f
		};
	}

	void REFify() {
		uint iStart = 0;
		for (uint j = 0; j < n && iStart < m; j++) {
			uint i;
			for (i = iStart; data[i][j] == 0 && i < m; i++) 
			{}
			if (i == m) {
				// all-zero column
				continue;
			}
			if (i != iStart) {
				// switch for row with non-zero leading entry
				for (uint k = j; k < n; k++) {
					std::swap(data[iStart][k], data[i][k]);
				}
			}

			T mult;
			for (i = iStart + 1; i < m; i++) {
				mult = data[i][j] / data[iStart][j];
				data[i][j] = 0;
				for (uint k = j + 1; k < n; k++) {
					data[i][k] -= data[iStart][k] * mult;
				}
			}
			iStart++;
		}
	}

	void RREFify() {
		REFify();
		T mult;
		uint i = 0;
		for (uint j = 0; j < n; j++) {
			mult = data[i][j];
			if (mult == 0) {
				continue;
			}
			data[i][j] = 1;
			for (uint k = j + 1; k < n; k++) {
				data[i][k] /= mult;
			}
			for (int i2 = i - 1; i2 >= 0; i2--) {
				mult = data[i2][j];
				if (mult == 0) {
					continue;
				}
				data[i2][j] = 0;
				for (uint k = j + 1; k < n; k++) {
					data[i2][k] -= data[i][k] * mult;
				}
			}
			i++;
		}

	}

	std::string To_String() const {
		std::stringstream ss;
		ss << "Matrix (" << m << ", " << n << ")\n";
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				ss << std::scientific << std::setprecision(6) << std::setw(14) << data[i][j];
			}
			ss << '\n';
		}
		return ss.str();
	}

	const T* Pointer() {
		return &data[0][0];
	}
};

using Matrix2f = Matrix<float, 2, 2>;
using Matrix3f = Matrix<float, 3, 3>;
using Matrix4f = Matrix<float, 4, 4>;

using Matrix2d = Matrix<double, 2, 2>;
using Matrix3d = Matrix<double, 3, 3>;
using Matrix4d = Matrix<double, 4, 4>;	

#endif