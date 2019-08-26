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
	T mData[m][n];

public:
	Matrix() 
	{}

	Matrix(T in_data[m][n]) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] = in_data[i][j];
			}
		}
	}

	template<typename... Args, typename = typename std::enable_if<sizeof...(Args) == m * n, void>::type>
	Matrix(Args... in_data) :
		mData{ in_data... }
	{}

	Matrix(const T* in_data) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] = in_data[i * n + j];
			}
		}
	}

	Matrix(const T in_data[m][n]) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] = in_data[i][j];
			}
		}
	}

	template<typename = typename std::enable_if<n == 1, void>::type>
	Matrix(const Vector<T, m>& vec) {
		for (uint i = 0; i < m; i++) {
			mData[i][0] = vec.get(i);
		}
	}

	Matrix(const Matrix<T, m, n>& mat) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] = mat.mData[i][j];
			}
		}
	}

	template<class T2>
	Matrix(const Matrix<T2, m, n>& mat) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] = (T)mat.mData[i][j];
			}
		}
	}

	template<typename = typename std::enable_if<n == 1, void>::type>
	explicit operator Vector<T, m> () {
		Vector<T, m> vOut = Vector<T, m>();
		for (uint i = 0; i < m; i++) {
			vOut[i] = mData[i][0];
		}
		return vOut;
	}

	T getElement(uint i, uint j) const {
		return mData[i][j];
	}

	Vector<T, n> getRow(uint i) const {
		return Vector<T, n>(&mData[i][0]);
	}

	Vector<T, m> getColumn(uint j) const {
		Vector<T, n> vOut();
		for (uint i = 0; i < m; i++) {
			vOut[i] = mData[i][j];
		}
		return vOut;
	} 

	bool isElementZero(uint i, uint j) const {
		return mData[i][j] == 0;
	}

	bool isRowZero(uint i) {
		for (uint j = 0; j < n; j++) {
			if (mData[i][j] != 0) {
				return false;
			}
		}
		return true;
	}

	bool isColumnZero(uint j) const {
		for (uint i = 0; i < m; i++) {
			if (mData[i][j] != 0) {
				return false;
			}
		}
		return true;
	}

	bool isSubmatrixZero(uint i1, uint j1, uint i2, uint j2) const {
		for (uint i = i1; i <= i2; i++) {
			for (uint j = j1; j < j2; j++) {
				if (mData[i][j] != 0) {
					return false;
				}
			}
		}
		return true;
	}

	void setElement(uint i, uint j, T value) {
		mData[i][j] = value;
	}

	void setRow(uint i, const Vector<T, n>& values) {
		for (uint j = 0; j < n; j++) {
			mData[i][j] = values[j];
		}
	}
	
	void setColumn(uint j, const Vector<T, m>& values) {
		for (uint i = 0; i < m; i++) {
			mData[i][j] = values.get(i);
		}
	}

	Matrix<T, m, n> operator+(const Matrix<T, m, n>& mat) const {
		Matrix<T, m, n> matOut = mData;
		matOut += mat;
		return matOut;
	}

	Matrix<T, m, n> operator-(const Matrix<T, m, n>& mat) const {
		Matrix<T, m, n> matOut = mData;
		matOut -= mat;
		return matOut;
	}

	template<uint p>
	Matrix<T, m, p> operator*(const Matrix<T, n, p>& mat) const {
		Matrix<T, m, p> matOut = Matrix<T, m, p>();
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < p; j++) {
				matOut.mData[i][j] = 0;
				for (uint k = 0; k < n; k++) {
					matOut.mData[i][j] += mData[i][k] * mat.mData[k][j];
				}
			}
		}
		return matOut;
	}

	Vector<T, m> operator*(const Vector<T, n>& vec) const {
		return Vector<T, m>((*this) * Matrix<T, n, 1>(vec));
	}

	Matrix<T, m, n> operator*(T scalar) const {
		Matrix<T, m, n> matOut = mData;
		matOut *= scalar;
		return matOut;
	}

	Matrix<T, m, n> operator/(T scalar) const {
		Matrix<T, m, n> matOut = mData;
		matOut /= scalar;
		return matOut;
	}

	void operator+=(const Matrix<T, m, n>& mat) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] += mat.mData[i][j];
			}
		}
	}

	void operator-=(const Matrix<T, m, n>& mat) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] -= mat.mData[i][j];
			}
		}
	}
	
	template<typename = typename std::enable_if<m == n, void>::type>
	void operator *=(const Matrix<T, m, n>& mat) {
		(*this) = Matrix<T, m, n>(mData) * mat;
	}

	void operator*=(T scalar) {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] *= scalar;
			}
		}
	}

	void operator/=(T scalar) {
		Matrix<T, m, n> matOut = Matrix<T, m, n>();
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				mData[i][j] *= scalar;
			}
		}
	}

	void transpose() {
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				if (i != j) {
					std::swap(mData[i][j], mData[j][i]);
				}
			}
		}
	}

	static Matrix<T, m, n> identity() {
		Matrix<T, m, n> matOut = Matrix<T, m, n>();
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				if (i == j) {
					matOut.mData[i][j] = 1;
				}
				else {
					matOut.mData[i][j] = 0;
				}
			}
		}
		return matOut;
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> rotation(const Vector<T, 3>& in_axis, T in_angle) {
		T s = sin(in_angle);
		T c = cos(in_angle);
		T omc = 1 - c;
		T x = in_axis.x();
		T y = in_axis.y();
		T z = in_axis.z();
		return {
			x * x * omc + c, x * y * omc - z * s, x * z * omc + y * s, (T)0,
			x * y * omc + z * s, y * y * omc + c, y * z * omc - x * s, (T)0,
			x * z * omc - y * s, y * z * omc + x * s, z * z * omc + c, (T)0,
			(T)0, (T)0, (T)0, (T)1
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> scale(const Vector<T, 3>& in_factor) {
		return {
			in_factor.x(), (T)0, (T)0, (T)0,
			(T)0, in_factor.y(), (T)0, (T)0,
			(T)0, (T)0, in_factor.z(), (T)0,
			(T)0, (T)0, (T)0, (T)1
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> translation(const Vector<T, 3>& in_translation) {
		return {
			(T)1, (T)0, (T)0, in_translation.x(),
			(T)0, (T)1, (T)0, in_translation.y(),
			(T)0, (T)0, (T)1, in_translation.z(),
			(T)0, (T)0, (T)0, (T)1
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> orthographic(Vector3f in_minima, Vector3f in_maxima) {
		return {
			2.0f / (in_maxima[0] - in_minima[0]), 0.0f, 0.0f, -(in_maxima[0] + in_minima[0]) / (in_maxima[0] - in_minima[0]),
			0.0f, 2.0f / (in_maxima[1] - in_minima[1]), 0.0f, -(in_maxima[1] + in_minima[1]) / (in_maxima[1] - in_minima[1]),
			0.0f, 0.0f, 2.0f / (in_maxima[2] - in_minima[2]), -(in_maxima[2] + in_minima[2]) / (in_maxima[2] - in_minima[2]),
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	template<typename = typename std::enable_if<m == 4 && n == 4, void>::type>
	static Matrix<T, m, n> perspective(float in_near, float in_far, float in_horizontalAngle, float aspectRatio) {
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
	static Matrix<T, m, n> perspective(Vector3f in_minima, Vector3f in_maxima) {
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
			for (i = iStart; mData[i][j] == 0 && i < m; i++) 
			{}
			if (i == m) {
				// all-zero column
				continue;
			}
			if (i != iStart) {
				// switch for row with non-zero leading entry
				for (uint k = j; k < n; k++) {
					std::swap(mData[iStart][k], mData[i][k]);
				}
			}

			T mult;
			for (i = iStart + 1; i < m; i++) {
				mult = mData[i][j] / mData[iStart][j];
				mData[i][j] = 0;
				for (uint k = j + 1; k < n; k++) {
					mData[i][k] -= mData[iStart][k] * mult;
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
			mult = mData[i][j];
			if (mult == 0) {
				continue;
			}
			mData[i][j] = 1;
			for (uint k = j + 1; k < n; k++) {
				mData[i][k] /= mult;
			}
			for (int i2 = i - 1; i2 >= 0; i2--) {
				mult = mData[i2][j];
				if (mult == 0) {
					continue;
				}
				mData[i2][j] = 0;
				for (uint k = j + 1; k < n; k++) {
					mData[i2][k] -= mData[i][k] * mult;
				}
			}
			i++;
		}

	}

	std::string toString() const {
		std::stringstream ss;
		ss << "Matrix (" << m << ", " << n << ")\n";
		for (uint i = 0; i < m; i++) {
			for (uint j = 0; j < n; j++) {
				ss << std::scientific << std::setprecision(6) << std::setw(14) << mData[i][j];
			}
			ss << '\n';
		}
		return ss.str();
	}

	const T* pointer() {
		return &mData[0][0];
	}
};

using Matrix2f = Matrix<float, 2, 2>;
using Matrix3f = Matrix<float, 3, 3>;
using Matrix4f = Matrix<float, 4, 4>;

using Matrix2d = Matrix<double, 2, 2>;
using Matrix3d = Matrix<double, 3, 3>;
using Matrix4d = Matrix<double, 4, 4>;	

#endif