#ifndef VECTOR_H
#define VECTOR_H

#include "definitions.h"
#include "misc.h"
#include <cmath>

template<class T, uint n>
class Vector {
	friend class Vector;
protected:
	T members[n];
public:
	Vector() 
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
		return *this * (1.0 - s) + v * s;
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

	string To_String() {
		stringstream ss;
		ss << "Vector (" << n << ")\n";
			for (uint i = 0; i < n; i++) {
				ss << scientific << setprecision(6) << setw(14) << members[i];
			}
			ss << '\n';
		return ss.str();
	}

	// Vector1-specific

	template<typename = std::enable_if_t<n >= 1, void>>
	T X() const {
		return members[0];
	}

	// Vector2-specific

	template<typename = std::enable_if_t<n >= 2, void>>
	T Y() const {
		return members[1];
	}

	template<typename = std::enable_if_t<n == 2, void>>
	void Rotate(T angle) {
		angle = angle * DEG_TO_RAD;
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

	template<typename = std::enable_if_t<n == 3, void>>
	void Rotate(const Vector<T, 3>& v) {
		T angle = v.Magnitude();
		if (angle == 0) {
			return;
		}
		Vector<T, 3> axis = v / angle;

		T sine = sin(angle);
		T cosine = cos(angle);
		T oneMinusCosine = (T)1 - cos(angle);

		T x = v.members[0];
		T y = v.members[1];
		T z = v.members[2];

		Vector<T, 3> old = members;

		members[0] =
			(cosine + x * x * oneMinusCosine) * old.X() +
			(x * y * oneMinusCosine - z * sine) * old.Y() +
			(x * z * oneMinusCosine + y * sine) * old.Z();
		members[1] =
			(y * x * oneMinusCosine + z * sine) * old.X() +
			(cosine + y * y * oneMinusCosine) * old.Y() +
			(y * z * oneMinusCosine - x * sine) * old.Z();
		members[2] =
			(z * x * oneMinusCosine - y * sine) * old.X() +
			(z * y * oneMinusCosine + x * sine) * old.Y() +
			(cosine + z * z * oneMinusCosine) * old.Z();
	}

	template<typename = std::enable_if_t<n == 3, void>>
	Vector<T, 3> Rotated(const Vector<T, 3>& v) const {
		Vector<T, 3> vOut = members;
		vOut.Rotate(v);
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

using Vector3i = Vector<int, 3>;
using Vector3f = Vector<float, 3>;
using Vector3d = Vector<double, 3>;

using Vector4i = Vector<int, 4>;
using Vector4f = Vector<float, 4>;
using Vector4d = Vector<double, 4>;

using Quaternionf = Vector<float, 4>;
using Quaterniond = Vector<double, 4>;

/*
template<class T>
class Vector2 {
public:
	union {
		T x;
		T b;
	};

	union {
		T y;
		T a;
	};


	Vector2() :
		x(0),
		y(0)
	{}


	Vector2(T x, T y) :
		x(x),
		y(y)
	{}


	template<class T2>
	Vector2(Vector2<T2> v) :
		x((T)v.x),
		y((T)v.y)
	{}


	template<class T2>
	Vector2(Vector3<T2> v) :
		x((T)v.x),
		y((T)v.y)
	{}


	Vector2<T> operator-() const {
		return Vector2<T>(-x, -y);
	}


	template<class T2>
	bool operator==(Vector2<T2> v) const {
		return x == (T)v.x && y == (T)v.y;
	}


	template<class T2>
	bool operator!=(Vector2<T2> v) const {
		return x != (T)v.x || y != (T)v.y;
	}


	template<class T2>
	Vector2<T> operator+(Vector2<T2> v) const {
		return Vector2<T>(x + (T)v.x, y + (T)v.y);
	}


	template<class T2>
	Vector2<T> operator-(Vector2<T2> v) const {
		return Vector2<T>(x - (T)v.x, y - (T)v.y);
	}


	template<class T2>
	Vector2<T> operator*(T2 s) const {
		return Vector2<T>(x * (T)s, y * (T)s);
	}


	template<class T2>
	Vector2<T> operator/(T2 s) const {
		return Vector2<T>(x / (T)s, y / (T)s);
	}


	template<class T2>
	void operator+=(Vector2<T2> v) {
		x += (T)v.x;
		y += (T)v.y;
	}


	template<class T2>
	void operator-=(Vector2<T2> v) {
		x -= (T)v.x;
		y -= (T)v.y;
	}


	template<class T2>
	void operator*=(T2 s) {
		x *= (T)s;
		y *= (T)s;
	}


	template<class T2>
	void operator/=(T2 s) {
		x /= (T)s;
		y /= (T)s;
	}

	bool Is_Zero() const {
		return x == 0.0 && y == 0.0;
	}


	T Theta() const {
		return (acos(x) + (y < 0) ? PI : 0.0) * RAD_TO_DEG;
	}


	template<class T2>
	T Theta(Vector2<T2> v) const {
		double t = acos(Normalized().Dot((T)v.Normalized())) * RAD_TO_DEG;
		return (t == t) ? t : 180.0;
	}


	template<class T2>
	void Rotate(T2 angle) {
		angle = angle * DEG_TO_RAD;
		double sine = sin(angle);
		double cosine = cos(angle);
		double tempx = x * cosine - y * sine;
		y = x * sine + y * cosine;
		x = tempx;
	}


	template<class T2>
	void Rotate(T2 angle, Vector2<T2> point) {
		*this -= point;
		Rotate(angle);
		*this += point;
	}


	template<class T2>
	Vector2<T>	Rotated(T2 angle) const {
		Vector2<T> v = *this;
		v.Rotate(angle);
		return v;
	}


	template<class T2>
	Vector2<T>	Rotated(T2 angle, Vector2<T2> point) const {
		Vector2<T> v = *this;
		v.Rotate(angle, point);
		return v;
	}


	T Magnitude() const {
		return sqrt(x * x + y * y);
	}


	void Normalize() {
		if (Is_Zero())
			return;
		T mag = Magnitude();
		x /= mag;
		y /= mag;
	}


	Vector2<T> Normalized() const {
		Vector2<T> v = *this;
		v.Normalize();
		return v;
	}


	template<class T2>
	T Dot(Vector2<T2> v) const {
		return x * (T)v.x + y * (T)v.y;
	}

	T Dot_Self() const {
		return x * x + y * y;
	}


	template<class T2>
	Vector2<T> Compwise(Vector2<T2> v) const {
		return Vector2<T>(x * (T)v.x, y * (T)v.y);
	}


	static Vector2<T> Random_Normalized() {
		return Vector2<T>(1, 0).Rotated(Rand_Double(0.0, 360.0));
	}
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

template<class T>
class Vector3 {
public:
	union {
		T x;
		T r;
	};

	union {
		T y;
		T g;
	};

	union {
		T z;
		T b;
	};


	Vector3() :
		x(0),
		y(0),
		z(0)
	{}


	Vector3(T x, T y, T z) :
		x(x),
		y(y),
		z(z)
	{}


	template<class T2>
	Vector3(Vector3<T2> v) :
		x((T)v.x),
		y((T)v.y),
		z((T)v.z)
	{}


	template<class T2>
	Vector3(Vector2<T2> v, T2 z) :
		x((T)v.x),
		y((T)v.y),
		z(z)
	{}


	template<class T2>
	Vector3(Vector4<T2> v) :
		x((T)v.x),
		y((T)v.y),
		z((T)v.z)
	{}


	Vector<T, 3> operator-() const {
		return Vector<T, 3>(-x, -y, -z);
	}


	template<class T2>
	bool operator==(Vector3<T2> v) const {
		return x == (T)v.x && y == (T)v.y && z == (T)v.z;
	}


	template<class T2>
	bool operator!=(Vector3<T2> v) const {
		return x != (T)v.x || y != (T)v.y || z != (T)v.z;
	}


	template<class T2>
	Vector<T, 3> operator+(Vector3<T2> v) const {
		return Vector<T, 3>(x + (T)v.x, y + (T)v.y, z + (T)v.z);
	}


	template<class T2>
	Vector<T, 3> operator-(Vector3<T2> v) const {
		return Vector<T, 3>(x - (T)v.x, y - (T)v.y, z - (T)v.z);
	}


	template<class T2>
	Vector<T, 3> operator*(T2 s) const {
		return Vector<T, 3>(x * (T)s, y * (T)s, z * (T)s);
	}


	template<class T2>
	Vector<T, 3> operator/(T2 s) const {
		return Vector<T, 3>(x / (T)s, y / (T)s, z / (T)s);
	}


	template<class T2>
	void operator+=(Vector3<T2> v) {
		x += (T)v.x;
		y += (T)v.y;
		z += (T)v.z;
	}


	template<class T2>
	void operator-=(Vector3<T2> v) {
		x -= (T)v.x;
		y -= (T)v.y;
		z -= (T)v.z;
	}


	template<class T2>
	void operator*=(T2 s) {
		x *= (T)s;
		y *= (T)s;
		z *= (T)s;
	}


	template<class T2>
	void operator/=(T2 s) {
		x /= (T)s;
		y /= (T)s;
		z /= (T)s;
	}

	bool Is_Zero() const {
		return x == 0.0 && y == 0.0 && z == 0.0;
	}


	T Theta() const {
		return acos(-z) * RAD_TO_DEG;;
	}


	template<class T2>
	T Theta(Vector3<T2> v) const {
		double t = acos(Normalized().Dot(v.Normalized())) * RAD_TO_DEG;
		return (t == t) ? t : 180.0;
	}


	template<class T2>
	void Rotate(Vector3<T2> v) {
		Rotate(v.Theta(), v.Normalized());
	}


	template<class T2>
	void Rotate(T2 angle, Vector3<T2> axis) {
		double theta = angle * DEG_TO_RAD;
		Vector3d cross = axis.Cross(*this);
		double c = cos(theta);
		double s = sin(theta);
		double dot = axis.Dot(*this);
		*this = *this * c + cross * s + axis * dot * (1.0 - c);
	}


	template<class T2>
	void Rotate(Vector3<T2> v, Vector3<T2> point) {
		Rotate((T)v.Theta(), (T)v.Normalized(), point);
	}


	template<class T2>
	void Rotate(T2 angle, Vector3<T2> axis, Vector3<T2> point) {
		*this -= point;
		Rotate(angle);
		*this += point;
	}


	template<class T2>
	Vector<T, 3> Rotated(Vector3<T2> v) const {
		return Rotated((T)v.Theta(), (T)v.Normalized());
	}


	template<class T2>
	Vector<T, 3>	Rotated(T2 angle, Vector3<T2> axis) const {
		Vector<T, 3> v = *this;
		(T)v.Rotate(angle);
		return v;
	}


	template<class T2>
	Vector<T, 3> Rotated(Vector3<T2> v, Vector3<T2> point) const {
		return Rotated((T)v.Theta(), (T)v.Normalized(), point);
	}


	template<class T2>
	Vector<T, 3>	Rotated(T2 angle, Vector3<T2> axis, Vector3<T2> point) const {
		Vector<T, 3> v = *this;
		(T)v.Rotate(angle, point);
		return v;
	}


	T Magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}


	void Normalize() {
		if (Is_Zero())
			return;
		T mag = Magnitude();
		x /= mag;
		y /= mag;
		z /= mag;
	}


	Vector<T, 3> Normalized() const {
		Vector<T, 3> v = *this;
		v.Normalize();
		return v;
	}


	template<class T2>
	T Dot(Vector3<T2> v) const {
		return x * (T)v.x + y * (T)v.y + z * (T)v.z;
	}

	T Dot_Self() const {
		return x * x + y * y + z * z;
	}

	template<class T2>
	Vector<T, 3> Cross(Vector3<T2> v) const {
		return Vector<T, 3>(y * (T)v.z - z * (T)v.y, z * (T)v.x - x * (T)v.z, x * (T)v.y - y * (T)v.x);
	}


	static Vector<T, 3> Random_Normalized() {
		double a = Rand_Double(-1.0, 1.0);
		double b = Rand_Double(0.0, 2.0 * PI);
		double c = sqrt(1 - a * a);
		return Vector<T, 3>(c * cos(b), c * sin(b), a);
	}


	template<class T2>
	Vector<T, 3> Compwise(Vector3<T2> v) const {
		return Vector<T, 3>(x * (T)v.x, y * (T)v.y, z * (T)v.z);
	}

	template<class T2>
	Vector<T, 3> Project(Vector3<T2> v) const {
		return v * (*this).Dot(v) / v.Dot_Self();
	}

	template<class T2>
	Vector<T, 3> Lazy_Project(Vector3<T2> v) const {
		return v * (*this).Dot(v);
	}

	template<class T2>
	T Project_Coeff(Vector3<T2> v) const {
		return (*this).Dot(v) / v.Dot_Self();
	}

	template<class T2>
	T Lazy_Project_Coeff(Vector3<T2> v) const {
		return (*this).Dot(v);
	}
};


using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

using Color3c = Vector3<uchar>;
using Color3f = Vector3<float>;

template<class T>
class Vector4 {
public:
	union {
		T x;
		T r;
	};

	union {
		T y;
		T g;
	};

	union {
		T z;
		T b;
	};

	union {
		T w;
		T a;
	};


	Vector4() :
		x(0),
		y(0),
		z(0),
		w(0)
	{}


	Vector4(T x, T y, T z, T w) :
		x(x),
		y(y),
		z(z),
		w(w)
	{}


	template<class T2>
	Vector4(Vector4<T2> v) :
		x((T)v.x),
		y((T)v.y),
		z((T)v.z),
		w((T)v.w)
	{}


	template<class T2>
	Vector4(Vector3<T2> v, T2 w) :
		x((T)v.x),
		y((T)v.y),
		z((T)v.y),
		w(w)
	{}


	template<class T2>
	bool operator==(Vector4<T2> v) const {
		return x == (T)v.x && y == (T)v.y && z == (T)v.z && w == (T)v.w;
	}


	template<class T2>
	bool operator!=(Vector4<T2> v) const {
		return x != (T)v.x || y != (T)v.y || z != (T)v.z || w != (T)v.w;
	}


	template<class T2>
	Vector4<T> operator+(Vector4<T2> v) const {
		return Vector4<T>(x + (T)v.x, y + (T)v.y, z + (T)v.z, w + (T)v.w);
	}


	template<class T2>
	Vector4<T> operator-(Vector4<T2> v) const {
		return Vector4<T>(x - (T)v.x, y - (T)v.y, z - (T)v.z, w - (T)v.w);
	}


	template<class T2>
	Vector4<T> operator*(T2 s) const {
		return Vector4<T>(x * (T)s, y * (T)s, z * (T)s, w * (T)s);
	}


	template<class T2>
	Vector4<T> operator/(T2 s) const {
		return Vector4<T>(x / (T)s, y / (T)s, z / (T)s, w / (T)s);
	}


	template<class T2>
	void operator+=(Vector4<T2> v) {
		x += (T)v.x;
		y += (T)v.y;
		z += (T)v.z;
		w += (T)v.w;
	}


	template<class T2>
	void operator-=(Vector4<T2> v) {
		x -= (T)v.x;
		y -= (T)v.y;
		z -= (T)v.z;
		w -= (T)v.w;
	}


	template<class T2>
	void operator*=(T2 s) {
		x *= (T)s;
		y *= (T)s;
		z *= (T)s;
		w *= (T)s;
	}


	template<class T2>
	void operator/=(T2 s) {
		x /= (T)s;
		y /= (T)s;
		z /= (T)s;
		w /= (T)s;
	}


	Vector4<T> operator-() const {
		return Vector4<T>(-x, -y, -z, -w);
	}


	bool Is_Zero() const {
		return x == 0 && y == 0 && z == 0 && w == 0;
	}


	T Magnitude() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}


	void Normalize() {
		if (Is_Zero())
			return;
		T mag = Magnitude();
		x /= mag;
		y /= mag;
		z /= mag;
	}


	Vector4<T> Normalized() const {
		Vector4<T> v = *this;
		(T)v.Normalize();
		return v;
	}


	template<class T2>
	T Dot(Vector4<T2> v) const {
		return x * (T)v.x + y * (T)v.y + z * (T)v.z + w * (T)v.w;
	}

	T Dot_Self() const {
		return x * x + y * y + z * z + w * W;
	}


	template<class T2>
	Vector4<T> Compwise(Vector4<T2> v) const {
		return Vector4<T>(x * (T)v.x, y * (T)v.y, z * (T)v.z, w * (T)v.w);
	}

};

using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;

using Color4c = Vector4<uchar>;
using Color4f = Vector4<float>;
*/
#endif

