#ifndef VECTOR_H
#define VECTOR_H

#include "definitions.h"
#include "misc.h"
#include <cmath>
#include <functional>

template<class T, uint n>
class Vector {
	friend class Vector;
	friend Vector<T, n> operator*(T s, const Vector<T, n>& v) {
		return v * s;
	}

	friend struct Hash;
	struct Hash {
		std::size_t operator()(const Vector<T, n>& v) {
			std::size_t out = 0;
			for (uint i = 0; i < n; i++) {
				out += std::hash<T>(v.mMembers[i]);
			}
			return out;
		}
	};

protected:
	T mMembers[n];

public:
	Vector() :
		mMembers{0}
	{}

	template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == n>>
	Vector(Args... in_members) :
		mMembers{(T)in_members...} 
	{}

	Vector(const T* in_members) {
		for (int i = 0; i < n; i++) {
			mMembers[i] = in_members[i];
		}
	}

	template<class T2>
	Vector(const Vector<T2, n>& v) {
		for (int i = 0; i < n; i++) {
			mMembers[i] = (T)v.mMembers[i];
		}
	}

	template<class T2>
	void operator=(const Vector<T2, n>& v) {
		for (int i = 0; i < n; i++) {
			mMembers[i] = (T)v.mMembers[i];
		}
	}

	explicit Vector(const Vector<T, n + 1> v) {
		for (int i = 0; i < n; i++) {
			mMembers[i] = v.mMembers[i];
		}
	}

	explicit Vector(const Vector<T, n - 1> v) {
		for (int i = 0; i < n - 1; i++) {
			mMembers[i] = v.mMembers[i];
		}
		mMembers[n - 1] = 0;
	}

	template<typename = std::enable_if_t<n == 1, void>>
	operator T() const {
		return *mMembers;
	}

	template<typename = std::enable_if_t<n == 1, void>>
	Vector(T in_source) {
		mMembers[0] = in_source;
	}

	Vector(const Vector<T, n - 1> v, T in_addMember) {
		for (int i = 0; i < n - 1; i++) {
			mMembers[i] = v.mMembers[i];
		}
		mMembers[n - 1] = in_addMember;
	}

	bool operator<(const Vector<T, n>& v) const {
		for (uint i = 0; i < n; i++) {
			if (mMembers[i] < v.mMembers[i]) return true;
			if (mMembers[i] > v.mMembers[i]) return false;
		}
		return false;
	}

	bool operator==(const Vector<T, n>& v) const {
		for (int i = 0; i < n; i++) {
			if (mMembers[i] != v.mMembers[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Vector<T, n>& v) const {
		for (int i = 0; i < n; i++) {
			if (mMembers[i] != v.mMembers[i]) {
				return true;
			}
		}
		return false;
	}

	Vector<T, n> operator-() const {
		Vector<T, n> vOut = mMembers;
		for (int i = 0; i < n; i++) {
			vOut.mMembers[i] = -vOut.mMembers[i];
		}
		return vOut;
	}

	Vector<T, n> operator+(const Vector<T, n>& v) const {
		Vector<T, n> vOut = mMembers;
		vOut += v;
		return vOut;
	}

	Vector<T, n> operator-(const Vector<T, n>& v) const {
		Vector<T, n> vOut = mMembers;
		vOut -= v;
		return vOut;
	}

	Vector<T, n> operator*(T s) const {
		Vector<T, n> vOut = mMembers;
		vOut *= s;
		return vOut;
	}

	Vector<T, n> operator/(T s) const {
		Vector<T, n> vOut = mMembers;
		vOut /= s;
		return vOut;
	}

	void operator+=(const Vector<T, n>& v) {
		for (int i = 0; i < n; i++) {
			mMembers[i] += v.mMembers[i];
		}
	}

	void operator-=(const Vector<T, n>& v) {
		for (int i = 0; i < n; i++) {
			mMembers[i] -= v.mMembers[i];
		}
	}

	void operator*=(T s) {
		for (int i = 0; i < n; i++) {
			mMembers[i] *= s;
		}
	}

	void operator/=(T s) {
		for (int i = 0; i < n; i++) {
			mMembers[i] /= s;
		}
	}

	T& operator[](uint i) {
		return mMembers[i];
	}

	const T& operator[](uint i) const {
		return mMembers[i];
	}

	T Get(uint i) const {
		return mMembers[i];
	}

	bool Has_NaN() {
		for (int i = 0; i < n; i++) {
			if (std::isnan(mMembers[i])) {
				return true;
			}
		}
		return false;
	}

	bool Is_Zero() const {
		for (int i = 0; i < n; i++) {
			if (mMembers[i] != 0) {
				return false;
			}
		}
		return true;
	}

	T Dot(const Vector<T, n>& v) const {
		T total = 0;
		for (int i = 0; i < n; i++) {
			total += mMembers[i] * v.mMembers[i];
		}
		return total;
	}

	T Dot_Self() const {
		T total = 0;
		for (int i = 0; i < n; i++) {
			total += mMembers[i] * mMembers[i];
		}
		return total;
	}

	T Magnitude() const {
		return sqrt(Dot_Self());
	}

	void Add_To_Magnitude(T s) {
		T magnitude = Magnitude();
		if (magnitude == 0) {
			return;
		}
		else {
			*this *= 1 + (s / magnitude);
		}
	}

	Vector<T, n> Normalized() const {
		Vector<T, n> vOut = mMembers;
		vOut.Normalize();
		return vOut;
	}

	void Normalize() {
		T magnitude = Magnitude();
		if (magnitude == 0) {
			return;
		}
		else {
			(*this) /= magnitude;
		}
	}

	Vector<T, n> Lerp(T s, const Vector<T, n> v) const {
		return *this * ((T)1 - s) + v * s;
	}

	Vector<T, n> Compwise(const Vector<T, n>& v) const {
		Vector<T, n> vOut = mMembers;
		for (int i = 0; i < n; i++) {
			vOut[i] *= v.mMembers[i];
		}
		return vOut;
	}

	T Component_Sum() const {
		T sum = 0;
		for (uint i = 0; i < n; i++) {
			sum += mMembers[i];
		}
		return sum;
	}

	T Component_Product() const {
		T product = 1;
		for (uint i = 0; i < n; i++) {
			product *= mMembers[i];
		}
		return product;
	}

	void Component_Invert() {
		for (uint i = 0; i < n; i++) {
			mMembers[i] = 1 / mMembers[i];
		}
	}

	Vector<T, n> Component_Inverted() const {
		Vector<T, n> vOut = mMembers;
		vOut.Component_Invert();
		return vOut;
	}

	T Max_Component() const {
		T result = mMembers[0];
		for (uint i = 1; i < n; i++) {
			if (mMembers[i] > result) {
				result = mMembers[i];
			}
		}
		return result;
	}

	T Min_Component() const {
		T result = mMembers[0];
		for (uint i = 1; i < n; i++) {
			if (mMembers[i] < result) {
				result = mMembers[i];
			}
		}
		return result;
	}

	T Theta(const Vector<T, n>& v) const {
		T dot = (*this).Normalized().Dot(v.Normalized());
		return acos(Clamp<T>(dot, -1, 1));
	}

	T Projection_Coeff(const Vector<T, n>& v) const {
		return (*this).Dot(v) / v.Dot_Self();
	}

	Vector<T, n> Projection(const Vector<T, n>& v) const {
		return v * Projection_Coeff(v);
	}

	const T* Pointer() const {
		return mMembers;
	}

	std::string To_String() const {
		std::stringstream ss;
		ss << "Vector (" << n << ")\n";
			for (uint i = 0; i < n; i++) {
				ss << std::scientific << std::setprecision(6) << std::setw(14) << mMembers[i];
			}
			ss << '\n';
		return ss.str();
	}

	// Vector1-specific

	template<typename = std::enable_if_t<n >= 1, void>>
	T X() const {
		return mMembers[0];
	}

	template<typename = std::enable_if_t<n >= 1, void>>
	T& X() {
		return mMembers[0];
	}

	// Vector2-specific

	template<typename = std::enable_if_t<n >= 2, void>>
	T Y() const {
		return mMembers[1];
	}

	template<typename = std::enable_if_t<n >= 2, void>>
	T& Y() {
		return mMembers[1];
	}

	template<typename = std::enable_if_t<n == 2, void>>
	void Rotate(T angle) {
		T sine = sin(angle);
		T cosine = cos(angle);
		T tempX = mMembers[0] * cosine - mMembers[1] * sine;
		mMembers[1] = mMembers[0] * sine + mMembers[1] * cosine;
		mMembers[0] = tempX;
	}

	template<typename = std::enable_if_t<n == 2, void>>
	Vector<T, 2> Rotated(T angle) const {
		Vector<T, 2> vOut = mMembers;
		vOut.Rotate(angle);
		return vOut;
	}

	template<typename = std::enable_if_t<n == 2, void>>
	Vector<T, 2> Orthogonal() const {
		return { mMembers[1], -mMembers[0] };
	}

	// Vector3-specific

	template<typename = std::enable_if_t<n >= 3, void>>
	T Z() const {
		return mMembers[2];
	}

	template<typename = std::enable_if_t<n >= 3, void>>
	T& Z() {
		return mMembers[2];
	}

	template<typename = std::enable_if_t<n == 3, void>>
	void Rotate(const Vector<T, 3>& v) {
		T angle = v.Magnitude();
		if (angle == 0) {
			return;
		}
		Vector<T, 3> axis = v / angle;
		Rotate(axis, angle);
	}

	template<typename = std::enable_if_t<n == 3, void>>
	Vector<T, 3> Rotated(const Vector<T, 3>& v) const {
		Vector<T, 3> vOut = mMembers;
		vOut.Rotate(v);
		return vOut;
	}

	template<typename = std::enable_if_t<n == 3, void>>
	void Rotate(const Vector<T, 3>& axis, T angle) {
		if (angle == 0) {
			return;
		}

		T sine = sin(angle);
		T cosine = cos(angle);

		*this = *this * cosine + axis.Cross(*this) * sine + axis * axis.Dot(*this) * ((T)1 - cosine);
	}

	template<typename = std::enable_if_t<n == 3, void>>
	Vector<T, 3> Rotated(const Vector<T, 3>& axis, T angle) const {
		Vector<T, 3> vOut = mMembers;
		vOut.Rotate(axis, angle);
		return vOut;
	}

	template<typename = std::enable_if_t<n == 3, void>>
	Vector<T, 3> Cross(const Vector<T, 3>& v) const {
		return Vector<T, 3>(
			mMembers[1] * v.mMembers[2] - mMembers[2] * v.mMembers[1],
			mMembers[2] * v.mMembers[0] - mMembers[0] * v.mMembers[2],
			mMembers[0] * v.mMembers[1] - mMembers[1] * v.mMembers[0]
			);
	}

	// Vector4-specific

	template<typename = std::enable_if_t<n >= 4, void>>
	T W() const {
		return mMembers[3];
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	T& W() {
		return mMembers[3];
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	inline Vector<T, 4> Hamilton(const Vector<T, 4>& v) const {
		return Vector<T, 4>(
			mMembers[3] * v.mMembers[0] + mMembers[0] * v.mMembers[3] + mMembers[1] * v.mMembers[2] - mMembers[2] * v.mMembers[1],
			mMembers[3] * v.mMembers[1] - mMembers[0] * v.mMembers[2] + mMembers[1] * v.mMembers[3] + mMembers[2] * v.mMembers[0],
			mMembers[3] * v.mMembers[2] + mMembers[0] * v.mMembers[1] - mMembers[1] * v.mMembers[0] + mMembers[2] * v.mMembers[3],
			mMembers[3] * v.mMembers[3] - mMembers[0] * v.mMembers[0] - mMembers[1] * v.mMembers[1] - mMembers[2] * v.mMembers[2]
		);
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	Vector<T, 4> Inverse() const {
		Vector<T, 4> vOut = mMembers;
		vOut.Invert();
		return vOut;
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	void Invert() {
		Conjugate();
		(*this) /= Dot_Self();
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	Vector<T, 4> Conjugated() const {
		Vector<T, 4> vOut = mMembers;
		vOut.Conjugate();
		return vOut;
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	void Conjugate() {
		mMembers[0] = -mMembers[0];
		mMembers[1] = -mMembers[1];
		mMembers[2] = -mMembers[2];
	}
};

using Vector2ui = Vector<uint, 2>;
using Vector2i = Vector<int, 2>;
using Vector2f = Vector<float, 2>;
using Vector2d = Vector<double, 2>;

using Vector3ui = Vector<uint, 3>;
using Vector3i = Vector<int, 3>;
using Vector3f = Vector<float, 3>;
using Vector3d = Vector<double, 3>;

using Vector4i = Vector<int, 4>;
using Vector4f = Vector<float, 4>;
using Vector4d = Vector<double, 4>;

using Quaternionf = Vector<float, 4>;
using Quaterniond = Vector<double, 4>;

#endif

