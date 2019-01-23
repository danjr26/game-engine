#ifndef VECTOR_H
#define VECTOR_H

#include "definitions.h"
#include "misc.h"
#include <cmath>

template<class T, uint n>
class Vector {
	friend class Vector;
	friend Vector<T, n> operator*(T s, const Vector<T, n>& v) {
		return v * s;
	}

protected:
	T members[n];

public:
	Vector() :
		members{0}
	{}

	template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == n>>
	Vector(Args... in_members) :
		members{(T)in_members...} 
	{}

	Vector(const T* in_members) {
		for (int i = 0; i < n; i++) {
			members[i] = in_members[i];
		}
	}

	template<class T2>
	Vector(const Vector<T2, n>& v) {
		for (int i = 0; i < n; i++) {
			members[i] = (T)v.members[i];
		}
	}

	template<class T2>
	void operator=(const Vector<T2, n>& v) {
		for (int i = 0; i < n; i++) {
			members[i] = (T)v.members[i];
		}
	}

	explicit Vector(const Vector<T, n + 1> v) {
		for (int i = 0; i < n; i++) {
			members[i] = v.members[i];
		}
	}

	explicit Vector(const Vector<T, n - 1> v) {
		for (int i = 0; i < n - 1; i++) {
			members[i] = v.members[i];
		}
		members[n - 1] = 0;
	}

	template<typename = std::enable_if_t<n == 1, void>>
	operator T() const {
		return *members;
	}

	template<typename = std::enable_if_t<n == 1, void>>
	Vector(T in_source) {
		members[0] = in_source;
	}

	Vector(const Vector<T, n - 1> v, T in_addMember) {
		for (int i = 0; i < n - 1; i++) {
			members[i] = v.members[i];
		}
		members[n - 1] = in_addMember;
	}

	bool operator==(const Vector<T, n>& v) const {
		for (int i = 0; i < n; i++) {
			if (members[i] != v.members[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Vector<T, n>& v) const {
		for (int i = 0; i < n; i++) {
			if (members[i] != v.members[i]) {
				return true;
			}
		}
		return false;
	}

	Vector<T, n> operator-() const {
		Vector<T, n> vOut = members;
		for (int i = 0; i < n; i++) {
			vOut.members[i] = -vOut.members[i];
		}
		return vOut;
	}

	Vector<T, n> operator+(const Vector<T, n>& v) const {
		Vector<T, n> vOut = members;
		vOut += v;
		return vOut;
	}

	Vector<T, n> operator-(const Vector<T, n>& v) const {
		Vector<T, n> vOut = members;
		vOut -= v;
		return vOut;
	}

	Vector<T, n> operator*(T s) const {
		Vector<T, n> vOut = members;
		vOut *= s;
		return vOut;
	}

	Vector<T, n> operator/(T s) const {
		Vector<T, n> vOut = members;
		vOut /= s;
		return vOut;
	}

	void operator+=(const Vector<T, n>& v) {
		for (int i = 0; i < n; i++) {
			members[i] += v.members[i];
		}
	}

	void operator-=(const Vector<T, n>& v) {
		for (int i = 0; i < n; i++) {
			members[i] -= v.members[i];
		}
	}

	void operator*=(T s) {
		for (int i = 0; i < n; i++) {
			members[i] *= s;
		}
	}

	void operator/=(T s) {
		for (int i = 0; i < n; i++) {
			members[i] /= s;
		}
	}

	T& operator[](uint i) {
		return members[i];
	}

	const T& operator[](uint i) const {
		return members[i];
	}

	T Get(uint i) const {
		return members[i];
	}

	bool Has_NaN() {
		for (int i = 0; i < n; i++) {
			if (std::isnan(members[i])) {
				return true;
			}
		}
		return false;
	}

	bool Is_Zero() const {
		for (int i = 0; i < n; i++) {
			if (members[i] != 0) {
				return false;
			}
		}
		return true;
	}

	T Dot(const Vector<T, n>& v) const {
		T total = 0;
		for (int i = 0; i < n; i++) {
			total += members[i] * v.members[i];
		}
		return total;
	}

	T Dot_Self() const {
		T total = 0;
		for (int i = 0; i < n; i++) {
			total += members[i] * members[i];
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
		Vector<T, n> vOut = members;
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
		Vector<T, n> vOut = members;
		for (int i = 0; i < n; i++) {
			vOut[i] *= v.members[i];
		}
		return vOut;
	}

	T Component_Sum() const {
		T sum = 0;
		for (uint i = 0; i < n; i++) {
			sum += members[i];
		}
		return sum;
	}

	T Component_Product() const {
		T product = 1;
		for (uint i = 0; i < n; i++) {
			product *= members[i];
		}
		return product;
	}

	void Component_Invert() {
		for (uint i = 0; i < n; i++) {
			members[i] = 1 / members[i];
		}
	}

	Vector<T, n> Component_Inverted() const {
		Vector<T, n> vOut = members;
		vOut.Component_Invert();
		return vOut;
	}

	T Max_Component() const {
		T result = members[0];
		for (uint i = 1; i < n; i++) {
			if (members[i] > result) {
				result = members[i];
			}
		}
		return result;
	}

	T Min_Component() const {
		T result = members[0];
		for (uint i = 1; i < n; i++) {
			if (members[i] < result) {
				result = members[i];
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
		return members;
	}

	std::string To_String() const {
		std::stringstream ss;
		ss << "Vector (" << n << ")\n";
			for (uint i = 0; i < n; i++) {
				ss << std::scientific << std::setprecision(6) << std::setw(14) << members[i];
			}
			ss << '\n';
		return ss.str();
	}

	// Vector1-specific

	template<typename = std::enable_if_t<n >= 1, void>>
	T X() const {
		return members[0];
	}

	template<typename = std::enable_if_t<n >= 1, void>>
	T& X() {
		return members[0];
	}

	// Vector2-specific

	template<typename = std::enable_if_t<n >= 2, void>>
	T Y() const {
		return members[1];
	}

	template<typename = std::enable_if_t<n >= 2, void>>
	T& Y() {
		return members[1];
	}

	template<typename = std::enable_if_t<n == 2, void>>
	void Rotate(T angle) {
		T sine = sin(angle);
		T cosine = cos(angle);
		T tempX = members[0] * cosine - members[1] * sine;
		members[1] = members[0] * sine + members[1] * cosine;
		members[0] = tempX;
	}

	template<typename = std::enable_if_t<n == 2, void>>
	Vector<T, 2> Rotated(T angle) const {
		Vector<T, 2> vOut = members;
		vOut.Rotate(angle);
		return vOut;
	}

	template<typename = std::enable_if_t<n == 2, void>>
	Vector<T, 2> Orthogonal() const {
		return { members[1], -members[0] };
	}

	// Vector3-specific

	template<typename = std::enable_if_t<n >= 3, void>>
	T Z() const {
		return members[2];
	}

	template<typename = std::enable_if_t<n >= 3, void>>
	T& Z() {
		return members[2];
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
		Vector<T, 3> vOut = members;
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
		Vector<T, 3> vOut = members;
		vOut.Rotate(axis, angle);
		return vOut;
	}

	template<typename = std::enable_if_t<n == 3, void>>
	Vector<T, 3> Cross(const Vector<T, 3>& v) const {
		return Vector<T, 3>(
			members[1] * v.members[2] - members[2] * v.members[1],
			members[2] * v.members[0] - members[0] * v.members[2],
			members[0] * v.members[1] - members[1] * v.members[0]
			);
	}

	// Vector4-specific

	template<typename = std::enable_if_t<n >= 4, void>>
	T W() const {
		return members[3];
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	T& W() {
		return members[3];
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	inline Vector<T, 4> Hamilton(const Vector<T, 4>& v) const {
		return Vector<T, 4>(
			members[3] * v.members[0] + members[0] * v.members[3] + members[1] * v.members[2] - members[2] * v.members[1],
			members[3] * v.members[1] - members[0] * v.members[2] + members[1] * v.members[3] + members[2] * v.members[0],
			members[3] * v.members[2] + members[0] * v.members[1] - members[1] * v.members[0] + members[2] * v.members[3],
			members[3] * v.members[3] - members[0] * v.members[0] - members[1] * v.members[1] - members[2] * v.members[2]
		);
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	Vector<T, 4> Inverse() const {
		Vector<T, 4> vOut = members;
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
		Vector<T, 4> vOut = members;
		vOut.Conjugate();
		return vOut;
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	void Conjugate() {
		members[0] = -members[0];
		members[1] = -members[1];
		members[2] = -members[2];
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

