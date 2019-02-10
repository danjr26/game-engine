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
			mMembers[i] = (uchar)(in_source.get(i) * 255.0f);
		}
	}

	template<typename = typename std::enable_if<std::is_same<float, T>::value, void>::type>
	Color(const Color<uchar, n>& in_source) {
		for (uint i = 0; i < n; i++) {
			mMembers[i] = ((float)in_source.get(i) / 255.0f);
		}
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T r() const {
		return mMembers[0];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T& r() {
		return mMembers[0];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T g() const {
		return mMembers[1];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T& g() {
		return mMembers[1];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T b() const {
		return mMembers[2];
	}

	template<typename = typename std::enable_if<n == 3 || n == 4, void>::type>
	T& b() {
		return mMembers[2];
	}

	template<typename = typename std::enable_if<n == 4, void>::type>
	T a() const {
		return mMembers[3];
	}

	template<typename = typename std::enable_if<n == 4, void>::type>
	T& a() {
		return mMembers[3];
	}

	T dot(const Vector<T, n>& v) const = delete;
	T dotSelf() const = delete;
	T magnitude() const = delete;
	Vector<T, n> normalized() const = delete;
	void normalize() = delete;
	T theta(const Vector<T, n>& v) const = delete;
};

using ColorRGBc = Color<uchar, 3>;
using ColorRGBf = Color<float, 3>;
using ColorRGBAc = Color<uchar, 4>;
using ColorRGBAf = Color<float, 4>;

#endif