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

template<class T>
struct NeuterableDeleter {
	bool neuter;

	NeuterableDeleter() : neuter(false) {};

	void operator()(T* in_toDelete) const {
		if (!neuter) delete in_toDelete;
	}
};

template<class T>
using neuterable_ptr = std::unique_ptr<T, NeuterableDeleter<T>>;

template<class T>
inline T Clamp(T n, T floor, T ceiling) {
	if (n < floor)
		return floor;
	if (n > ceiling)
		return ceiling;
	return n;
}

template<class T>
inline T Clamp_Floor(T n, T floor) {
	if (n < floor)
		return floor;
	return n;
}

template<class T>
inline T Clamp_Ceiling(T n, T ceiling) {
	if (n > ceiling)
		return ceiling;
	return n;
}

template<class T>
inline T Mean(T a, T b) {
	return (a + b) * 0.5;
}

template<class T>
inline bool Between_Exc(T a, T b1, T b2) {
	if (b1 < b2) {
		return a > b1 && a < b2;
	}
	else {
		return a > b2 && a < b1;
	}
}

template<class T>
bool Between_Inc(T a, T b1, T b2) {
	if (b1 < b2) {
		return a >= b1 && a <= b2;
	}
	else {
		return a >= b2 && a <= b1;
	}
}

template<class T>
inline bool Ceq_Switch(T a, T b, bool geq) {
	return (geq) ? a >= b : a <= b;
}

template<class T>
inline T Min(T a, T b) {
	if (b < a) {
		return b;
	}
	else {
		return a;
	}
}

template<class T>
inline T Min(std::initializer_list<T> vList) {
	if (vList.size() == 0) return 0;
	T out = *(vList.begin());
	for (auto it = vList.begin() + 1; it < vList.end(); it++) {
		if (*it < out) out = *it;
	}
	return out;
}

template<class T>
inline uint Min_Index(std::initializer_list<T> vList) {
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
inline T Min(T* vList, uint n) {
	if (n == 0) return 0;
	T out = vList[0];
	for (uint i = 1; i < n; i++) {
		if (vList[i] < out) out = vList[i];
	}
	return out;
}

template<class T>
inline uint Min_Index(T* vList, uint n) {
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
inline T Min(std::vector<T> vList) {
	if (vList.size() == 0) return 0;
	T out = vList[0];
	for (auto it = vList.begin() + 1; it < vList.end(); it++) {
		if (*it < out) out = *it;
	}
	return out;
}

template<class T> 
inline T Max(T a, T b) {
	if (b > a) {
		return b;
	}
	else {
		return a;
	}
}

template<class T>
inline T Max(std::initializer_list<T> vList) {
	if (vList.size() == 0) return 0;
	T out = *(vList.begin());
	for (auto it = vList.begin() + 1; it < vList.end(); it++) {
		if (*it > out) out = *it;
	}
	return out;
}

template<class T>
inline T Max(T* vList, uint n) {
	if (n == 0) return 0;
	T out = vList[0];
	for (uint i = 1; i < n; i++) {
		if (vList[i] > out) out = vList[i];
	}
	return out;
}

template<class T>
inline T Max(std::vector<T> vList) {
	if (vList.size() == 0) return 0;
	T out = vList[0];
	for (auto it = vList.begin() + 1; it < vList.end(); it++) {
		if (*it > out) out = *it;
	}
	return out;
}

template<class T>
inline void Min_Max(const std::initializer_list<T>& vList, T& out_min, T& out_max) {
	if (vList.size() == 0) return;
	out_min = *(vList.begin());
	out_max = out_min;
	for (auto it = vList.begin() + 1; it < vList.end(); it++) {
		if (*it < out_min) out_min = *it;
		if (*it > out_max) out_max = *it;
	}
}

template<class T>
inline void Min_Max(T* vList, uint n, T& out_min, T& out_max) {
	if (n == 0) return;
	out_min = vList[0];
	out_max = out_min;
	for (uint i = 1; i < n; i++) {
		if (vList[i] < out_min) out_min = vList[i];
		if (vLost[i] < out_max) out_max = vList[i];
	}
}

template<class T>
inline void Min_Max(const std::vector<T>& vList, T& out_min, T& out_max) {
	if (vList.size() == 0) return;
	out_min = *(vList.begin());
	out_max = out_min;
	for (auto it = vList.begin() + 1; it < vList.end(); it++) {
		if (*it < out_min) out_min = *it;
		if (*it > out_max) out_max = *it;
	}
}

template<class T>	
inline T Lerp(T a, T b, T t) {
	return a * (1 - t) + b * t;
}

template<class T>
inline T Inv_Lerp(T a, T b, T t) {
	return (a == b) ? 0 : (t - a) / (b - a);
}

template<class T>
inline T Lerp(T a, T b, T t, T ta, T tb) {
	return Lerp(a, b, Inv_Lerp(ta, tb, t));
}

template<class T>
inline T Cuberp(T a, T b, T t) {
	return Lerp<T>(a, b, t * t * (3 - 2 * t));
}

template<class T>
inline T Sign(T n) {
	return (n > (T)0) ? (T)1 : ((n < (T)0) ? (T)-1 : (T)0);
}

template<class C, class T>
inline T Sum(C& values, uint begin, uint end) {
	T s = 0;
	for (uint i = begin; i < end; i++) s += values[i];
	return s;
}

template<class C, class T>
inline T Sum(C& values, uint n) {
	T s = 0;
	for (uint i = 0; i < n; i++) s += values[i];
	return s;
}



template<class T>
T Random();

template<> bool Random<bool>();
template<> uint Random<uint>();
template<> int Random<int>();
template<> ullong Random<ullong>();
template<> llong Random<llong>();

template<class T>
T Random(T high);

template<> uint Random<uint>(uint);
template<> int Random<int>(int);
template<> ullong Random<ullong>(ullong);
template<> llong Random<llong>(llong);
template<> float Random<float>(float);
template<> double Random<double>(double);

template<class T>
T Random(T low, T high);

#endif 

