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

	T get(uint i) const {
		return mMembers[i];
	}

	bool hasNan() {
		for (int i = 0; i < n; i++) {
			if (std::isnan(mMembers[i])) {
				return true;
			}
		}
		return false;
	}

	bool isZero() const {
		for (int i = 0; i < n; i++) {
			if (mMembers[i] != 0) {
				return false;
			}
		}
		return true;
	}

	T dot(const Vector<T, n>& v) const {
		T total = 0;
		for (int i = 0; i < n; i++) {
			total += mMembers[i] * v.mMembers[i];
		}
		return total;
	}

	T dotSelf() const {
		T total = 0;
		for (int i = 0; i < n; i++) {
			total += mMembers[i] * mMembers[i];
		}
		return total;
	}

	T magnitude() const {
		return sqrt(dotSelf());
	}

	void addToMagnitude(T s) {
		T mag = magnitude();
		if (mag == 0) {
			return;
		}
		else if (mag < -s) {
			*this = Vector<T, n>();
		}
		else {
			*this *= 1 + (s / mag);
		}
	}

	Vector<T, n> normalized() const {
		Vector<T, n> vOut = mMembers;
		vOut.normalize();
		return vOut;
	}

	void normalize() {
		T mag = magnitude();
		if (mag == 0) {
			return;
		}
		else {
			(*this) /= mag;
		}
	}

	Vector<T, n> withMagnitude(T m) const {
		return this->normalized() * m;
	}

	Vector<T, n> lerp(const Vector<T, n> v, T t) const {
		return *this * ((T)1 - t) + v * t;
	}

	Vector<T, n> compwise(const Vector<T, n>& v) const {
		Vector<T, n> vOut = mMembers;
		for (int i = 0; i < n; i++) {
			vOut[i] *= v.mMembers[i];
		}
		return vOut;
	}

	T componentSum() const {
		T sum = 0;
		for (uint i = 0; i < n; i++) {
			sum += mMembers[i];
		}
		return sum;
	}

	T componentProduct() const {
		T product = 1;
		for (uint i = 0; i < n; i++) {
			product *= mMembers[i];
		}
		return product;
	}

	void componentInvert() {
		for (uint i = 0; i < n; i++) {
			mMembers[i] = 1 / mMembers[i];
		}
	}

	Vector<T, n> componentInverted() const {
		Vector<T, n> vOut = mMembers;
		vOut.componentInvert();
		return vOut;
	}

	T maxComponent() const {
		T result = mMembers[0];
		for (uint i = 1; i < n; i++) {
			if (mMembers[i] > result) {
				result = mMembers[i];
			}
		}
		return result;
	}

	T minComponent() const {
		T result = mMembers[0];
		for (uint i = 1; i < n; i++) {
			if (mMembers[i] < result) {
				result = mMembers[i];
			}
		}
		return result;
	}

	void clamp(const Vector<T, n>& low, const Vector<T, n> high) {
		for (uint i = 0; i < n; i++) {
			mMembers[i] = GEUtil::clamp(mMembers[i], low.mMembers[i], high.mMembers[i]);
		}
	}

	T theta(const Vector<T, n>& v) const {
		T dot = (*this).normalized().dot(v.normalized());
		return acos(GEUtil::clamp<T>(dot, -1, 1));
	}

	T projectionCoeff(const Vector<T, n>& v) const {
		return (*this).dot(v) / v.dotSelf();
	}

	Vector<T, n> projection(const Vector<T, n>& v) const {
		return v * projectionCoeff(v);
	}

	const T* pointer() const {
		return mMembers;
	}

	std::string toString() const {
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
	T x() const {
		return mMembers[0];
	}

	template<typename = std::enable_if_t<n >= 1, void>>
	T& x() {
		return mMembers[0];
	}

	// Vector2-specific

	template<typename = std::enable_if_t<n >= 2, void>>
	T y() const {
		return mMembers[1];
	}

	template<typename = std::enable_if_t<n >= 2, void>>
	T& y() {
		return mMembers[1];
	}

	template<typename = std::enable_if_t<n == 2, void>>
	Vector<T, n> withTheta(T t) {
		T mag = magnitude();
		return Vector<T, n>(cos(t), sin(t)) * mag;
	}

	template<typename = std::enable_if_t<n == 2, void>>
	T ccwTheta() const {
		if (mMembers[1] > 0) {
			return theta(Vector<T, n>(1, 0));
		}
		else if (mMembers[1] < 0) {
			return theta(Vector<T, n>(1, 0)) + 2.0 * PI;
		}
		else if (mMembers[0] > 0) {
			return (T)0;
		}
		else if (mMembers[0] < 0) {
			return (T)PI;
		}
		else {
			return (T)0;
		}
	}

	template<typename = std::enable_if_t<n == 2, void>>
	T ccwTheta(const Vector<T, n>& v) const {
		T diff = v.ccwTheta() - ccwTheta();
		return (diff >= 0) ? diff : (diff + 2.0 * PI);
	}

	template<typename = std::enable_if_t<n == 2, void>>
	void rotate(T angle) {
		T sine = sin(angle);
		T cosine = cos(angle);
		T tempX = mMembers[0] * cosine - mMembers[1] * sine;
		mMembers[1] = mMembers[0] * sine + mMembers[1] * cosine;
		mMembers[0] = tempX;
	}

	template<typename = std::enable_if_t<n == 2, void>>
	Vector<T, 2> rotated(T angle) const {
		Vector<T, 2> vOut = mMembers;
		vOut.rotate(angle);
		return vOut;
	}

	template<typename = std::enable_if_t<n == 2, void>>
	Vector<T, 2> orthogonal() const {
		return { mMembers[1], -mMembers[0] };
	}

	// Vector3-specific

	template<typename = std::enable_if_t<n >= 3, void>>
	T z() const {
		return mMembers[2];
	}

	template<typename = std::enable_if_t<n >= 3, void>>
	T& z() {
		return mMembers[2];
	}

	template<typename = std::enable_if_t<n == 3, void>>
	void rotate(const Vector<T, 3>& v) {
		T angle = v.magnitude();
		if (angle == 0) {
			return;
		}
		Vector<T, 3> axis = v / angle;
		rotate(axis, angle);
	}

	template<typename = std::enable_if_t<n == 3, void>>
	Vector<T, 3> rotated(const Vector<T, 3>& v) const {
		Vector<T, 3> vOut = mMembers;
		vOut.rotate(v);
		return vOut;
	}

	template<typename = std::enable_if_t<n == 3, void>>
	void rotate(const Vector<T, 3>& axis, T angle) {
		if (angle == 0) {
			return;
		}

		T sine = sin(angle);
		T cosine = cos(angle);

		*this = *this * cosine + axis.cross(*this) * sine + axis * axis.dot(*this) * ((T)1 - cosine);
	}

	template<typename = std::enable_if_t<n == 3, void>>
	Vector<T, 3> rotated(const Vector<T, 3>& axis, T angle) const {
		Vector<T, 3> vOut = mMembers;
		vOut.rotate(axis, angle);
		return vOut;
	}

	template<typename = std::enable_if_t<n == 3, void>>
	Vector<T, 3> cross(const Vector<T, 3>& v) const {
		return Vector<T, 3>(
			mMembers[1] * v.mMembers[2] - mMembers[2] * v.mMembers[1],
			mMembers[2] * v.mMembers[0] - mMembers[0] * v.mMembers[2],
			mMembers[0] * v.mMembers[1] - mMembers[1] * v.mMembers[0]
			);
	}

	// Vector4-specific

	template<typename = std::enable_if_t<n >= 4, void>>
	T w() const {
		return mMembers[3];
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	T& w() {
		return mMembers[3];
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	inline Vector<T, 4> hamilton(const Vector<T, 4>& v) const {
		return Vector<T, 4>(
			mMembers[3] * v.mMembers[0] + mMembers[0] * v.mMembers[3] + mMembers[1] * v.mMembers[2] - mMembers[2] * v.mMembers[1],
			mMembers[3] * v.mMembers[1] - mMembers[0] * v.mMembers[2] + mMembers[1] * v.mMembers[3] + mMembers[2] * v.mMembers[0],
			mMembers[3] * v.mMembers[2] + mMembers[0] * v.mMembers[1] - mMembers[1] * v.mMembers[0] + mMembers[2] * v.mMembers[3],
			mMembers[3] * v.mMembers[3] - mMembers[0] * v.mMembers[0] - mMembers[1] * v.mMembers[1] - mMembers[2] * v.mMembers[2]
		);
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	Vector<T, 4> inverse() const {
		Vector<T, 4> vOut = mMembers;
		vOut.invert();
		return vOut;
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	void invert() {
		conjugate();
		(*this) /= dotSelf();
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	Vector<T, 4> conjugated() const {
		Vector<T, 4> vOut = mMembers;
		vOut.conjugate();
		return vOut;
	}

	template<typename = std::enable_if_t<n >= 4, void>>
	void conjugate() {
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

