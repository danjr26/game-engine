#ifndef MISC_H
#define MISC_H

#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

#include "windows_misc.h"
#include "definitions.h"

#define fail() exit(fprintf(stdout, "error @%s:%d\nwin: %s", __FILE__, __LINE__, getWindowsErrorMessage().c_str()) & std::cin.get())

inline void fileToString(const std::string& i_filename, std::string& o_data) {
	std::ifstream file;
	file.open(i_filename);
	if (!file.is_open()) fail();
	std::stringstream ss;
	ss << file.rdbuf();
	o_data = ss.str();
}

template<class T>
inline T clamp(T n, T floor, T ceiling) {
	if (n < floor)
		return floor;
	if (n > ceiling)
		return ceiling;
	return n;
}

template<class T>
inline T regress(T start, T step, T towards) {
	step = (step < 0) ? -step : step;
	if (start > towards + step) return start - step;
	else if (start < towards - step) return start + step;
	return towards;
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
inline bool betwInc(T a, T b1, T b2) {
	if (b1 < b2) {
		return a >= b1 && a <= b2;
	}
	else {
		return a >= b2 && a <= b1;
	}
}

template<class T>
inline T diff(T a, T b) {
	return (a > b) ? (a - b) : (b - a);
}

template<class T>
inline T closest(T t, T a, T b) {
	return (diff(t, a) > diff(t, b)) ? b : a;
}

template<class T>
inline T farthest(T t, T a, T b) {
	return (diff(t, a) < diff(t, b)) ? b : a;
}

template<class T>
inline int compare(T a, T b) {
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}

template<class T>
inline bool ceqSwitch(T a, T b, bool geq) {
	return (geq) ? a >= b : a <= b;
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
inline T cerp(T a, T b, T t) {
	return lerp<T>(a, b, t * t * (3 - 2 * t));
}

template<class T>
inline T qerp(T a, T b, T t) {
	return lerp<T>(a, b, t * t * t * (10 + t * (-15 + t * 6)));
}

template<class T>
inline T sign(T n) {
	return (n > (T)0) ? (T)1 : ((n < (T)0) ? (T)-1 : (T)0);
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

template<class T>
inline T gauss() {
	static std::default_random_engine generator;
	static std::normal_distribution<T> distribution;
	return distribution(generator);
}

#endif 

