#ifndef COLOR_H
#define COLOR_H

#include "vector.h"
#include "definitions.h"

template<class T, uint n, typename = typename std::enable_if<n >= 1 && n <= 4, void>::type>
class Color : public Vector<T, n> {
public:
	using Vector<T, n>::Vector;

	template<class T2> Color(const Color<T2, n>& v) = delete;

	Color() 
	{}

	template<typename = typename std::enable_if<std::is_same<uchar, T>::value, void>::type>
	Color(const Color<float, n>& in_source) {
		for (uint i = 0; i < n; i++) {
			mMembers[i] = (uchar)(in_source.Get(i) * 255.0f);
		}
	}

	template<typename = typename std::enable_if<std::is_same<float, T>::value, void>::type>
	Color(const Color<uchar, n>& in_source) {
		for (uint i = 0; i < n; i++) {
			mMembers[i] = ((float)in_source.Get(i) / 255.0f);
		}
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T R() const {
		return mMembers[0];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T& R() {
		return mMembers[0];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T G() const {
		return mMembers[1];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T& G() {
		return mMembers[1];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T B() const {
		return mMembers[2];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T& B() {
		return mMembers[2];
	}

	template<typename = typename std::enable_if<n == 4, void>::type>
	T A() const {
		return mMembers[3];
	}

	template<typename = typename std::enable_if<n == 4, void>::type>
	T& A() {
		return mMembers[3];
	}

	T Dot(const Vector<T, n>& v) const = delete;
	T Dot_Self() const = delete;
	T Magnitude() const = delete;
	Vector<T, n> Normalized() const = delete;
	void Normalize() = delete;
	T Theta(const Vector<T, n>& v) const = delete;
};

using ColorRGBc = Color<uchar, 3>;
using ColorRGBf = Color<float, 3>;
using ColorRGBAc = Color<uchar, 4>;
using ColorRGBAf = Color<float, 4>;

#endif