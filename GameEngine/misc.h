#ifndef MISC_H
#define MISC_H

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <functional>
#include <initializer_list>
#include <random>
#include <memory>
#include "definitions.h"

#undef min
#undef max

template<class T>
struct NeuterableDeleter {
	bool mNeuter;

	NeuterableDeleter() : mNeuter(false) {};

	void operator()(T* in_toDelete) const {
		if (!mNeuter) delete in_toDelete;
	}
};

template<class T>
using neuterable_ptr = std::unique_ptr<T, NeuterableDeleter<T>>;

namespace GEUtil {
	template<class T>
	inline T clamp(T n, T floor, T ceiling) {
		if (n < floor)
			return floor;
		if (n > ceiling)
			return ceiling;
		return n;
	}

	template<class T>
	inline T clampFloor(T n, T floor) {
		if (n < floor)
			return floor;
		return n;
	}

	template<class T>
	inline T clampCeiling(T n, T ceiling) {
		if (n > ceiling)
			return ceiling;
		return n;
	}

	template<class T>
	inline T mean(T a, T b) {
		return (a + b) * 0.5;
	}

	template<class T>
	inline bool betwExc(T a, T b1, T b2) {
		if (b1 < b2) {
			return a > b1 && a < b2;
		}
		else {
			return a > b2 && a < b1;
		}
	}

	template<class T>
	bool betwInc(T a, T b1, T b2) {
		if (b1 < b2) {
			return a >= b1 && a <= b2;
		}
		else {
			return a >= b2 && a <= b1;
		}
	}

	template<class T>
	inline bool ceqSwitch(T a, T b, bool geq) {
		return (geq) ? a >= b : a <= b;
	}

	template<class T>
	inline T min(T a, T b) {
		if (b < a) {
			return b;
		}
		else {
			return a;
		}
	}

	template<class T>
	inline T min(std::initializer_list<T> vList) {
		if (vList.size() == 0) return 0;
		T out = *(vList.begin());
		for (auto it = vList.begin() + 1; it < vList.end(); it++) {
			if (*it < out) out = *it;
		}
		return out;
	}

	template<class T>
	inline uint minIndex(std::initializer_list<T> vList) {
		if (vList.size() == 0) return 0;
		uint index = 0;
		T value = *(vList.begin());
		uint i = 1;
		for (auto it = vList.begin() + 1; it < vList.end(); it++) {
			if (*it < value) {
				value = *it;
				index = i;
			}
			i++;
		}
		return i;
	}

	template<class T>
	inline T min(T* vList, uint n) {
		if (n == 0) return 0;
		T out = vList[0];
		for (uint i = 1; i < n; i++) {
			if (vList[i] < out) out = vList[i];
		}
		return out;
	}

	template<class T>
	inline uint minIndex(T* vList, uint n) {
		if (n == 0) return 0;
		uint index = 0;
		T value = vList[0];
		for (uint i = 1; i < n; i++) {
			if (vList[i] < value) {
				value = vList[i];
				index = i;
			}
		}
		return index;
	}

	template<class T>
	inline T min(std::vector<T> vList) {
		if (vList.size() == 0) return 0;
		T out = vList[0];
		for (auto it = vList.begin() + 1; it < vList.end(); it++) {
			if (*it < out) out = *it;
		}
		return out;
	}

	template<class T>
	inline T max(T a, T b) {
		if (b > a) {
			return b;
		}
		else {
			return a;
		}
	}

	template<class T>
	inline T max(std::initializer_list<T> vList) {
		if (vList.size() == 0) return 0;
		T out = *(vList.begin());
		for (auto it = vList.begin() + 1; it < vList.end(); it++) {
			if (*it > out) out = *it;
		}
		return out;
	}

	template<class T>
	inline T max(T* vList, uint n) {
		if (n == 0) return 0;
		T out = vList[0];
		for (uint i = 1; i < n; i++) {
			if (vList[i] > out) out = vList[i];
		}
		return out;
	}

	template<class T>
	inline T max(std::vector<T> vList) {
		if (vList.size() == 0) return 0;
		T out = vList[0];
		for (auto it = vList.begin() + 1; it < vList.end(); it++) {
			if (*it > out) out = *it;
		}
		return out;
	}

	template<class T>
	inline void minMax(const std::initializer_list<T>& vList, T& out_min, T& out_max) {
		if (vList.size() == 0) return;
		out_min = *(vList.begin());
		out_max = out_min;
		for (auto it = vList.begin() + 1; it < vList.end(); it++) {
			if (*it < out_min) out_min = *it;
			if (*it > out_max) out_max = *it;
		}
	}

	template<class T>
	inline void minMax(T* vList, uint n, T& out_min, T& out_max) {
		if (n == 0) return;
		out_min = vList[0];
		out_max = out_min;
		for (uint i = 1; i < n; i++) {
			if (vList[i] < out_min) out_min = vList[i];
			if (vLost[i] < out_max) out_max = vList[i];
		}
	}

	template<class T>
	inline void minMax(const std::vector<T>& vList, T& out_min, T& out_max) {
		if (vList.size() == 0) return;
		out_min = *(vList.begin());
		out_max = out_min;
		for (auto it = vList.begin() + 1; it < vList.end(); it++) {
			if (*it < out_min) out_min = *it;
			if (*it > out_max) out_max = *it;
		}
	}

	template<class T>
	inline T lerp(T a, T b, T t) {
		return a * (1 - t) + b * t;
	}

	template<class T>
	inline T invLerp(T a, T b, T t) {
		return (a == b) ? 0 : (t - a) / (b - a);
	}

	template<class T>
	inline T lerp(T a, T b, T t, T ta, T tb) {
		return lerp(a, b, invLerp(ta, tb, t));
	}

	template<class T>
	inline T cuberp(T a, T b, T t) {
		return lerp<T>(a, b, t * t * (3 - 2 * t));
	}

	template<class T>
	inline T sign(T n) {
		return (n > (T)0) ? (T)1 : ((n < (T)0) ? (T)-1 : (T)0);
	}

	template<class C, class T>
	inline T sum(C& values, uint begin, uint end) {
		T s = 0;
		for (uint i = begin; i < end; i++) s += values[i];
		return s;
	}

	template<class C, class T>
	inline T sum(C& values, uint n) {
		T s = 0;
		for (uint i = 0; i < n; i++) s += values[i];
		return s;
	}

	template<class T>
	inline uint solveQuadratic(T a, T b, T c, T& x1, T& x2) {
		T disc = b * b - 4 * a * c;
		if (disc < 0) return 0;
		T sqrtDisc = sqrt(disc);
		T denom = 1 / (2 * a);
		x1 = (-b - sqrtDisc) * denom;
		x2 = (-b + sqrtDisc) * denom;
		return (disc == 0) ? 1 : 2;
	}

	template<class T>
	T random();

	template<> bool random<bool>();
	template<> uint random<uint>();
	template<> int random<int>();
	template<> ullong random<ullong>();
	template<> llong random<llong>();

	template<class T>
	T random(T high);

	template<> uint random<uint>(uint);
	template<> int random<int>(int);
	template<> ullong random<ullong>(ullong);
	template<> llong random<llong>(llong);
	template<> float random<float>(float);
	template<> double random<double>(double);

	template<class T>
	T random(T low, T high);
}

#endif 

