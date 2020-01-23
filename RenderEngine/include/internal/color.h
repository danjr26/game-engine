#ifndef COLOR_H
#define COLOR_H

#include "../../../Utilities/include/utilities.h"

template<class T, uint n, typename = typename std::enable_if_t<n >= 1 && n <= 4, void>>
class Color : public Vector<T, n> {
	using Vector<T, n>::Vector;
	using Vector<T, n>::mMembers;

public:
	T r() const {
		return mMembers[0];
	}

	T& r() {
		return mMembers[0];
	}

	template<typename = typename std::enable_if_t<n >= 2, void>>
	T g() const {
		return mMembers[1];
	}

	template<typename = typename std::enable_if_t<n >= 2, void>>
	T& g() {
		return mMembers[1];
	}

	template<typename = typename std::enable_if_t<n >= 3, void>>
	T b() const {
		return mMembers[2];
	}

	template<typename = typename std::enable_if_t<n >= 3, void>>
	T& b() {
		return mMembers[2];
	}

	template<typename = typename std::enable_if_t<n >= 4, void>>
	T a() const {
		return mMembers[3];
	}

	template<typename = typename std::enable_if_t<n >= 4, void>>
	T& a() {
		return mMembers[3];
	}

};

using Color3b = Color<uchar, 3>;
using Color3f = Color<float, 3>;
using Color4b = Color<uchar, 4>;
using Color4f = Color<float, 4>;

#endif