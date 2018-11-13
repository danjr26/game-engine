#include "misc.h"
#include "exceptions.h"

template<>
bool Random<bool>() {
	return Random<uint>(2) == 1;
}

template<>
uint Random<uint>() {
	static std::mt19937 engine;
	return engine();
}

template<>
int Random<int>() {
	return static_cast<int>(Random<uint>());
}

template<>
ullong Random<ullong>() {
	static std::mt19937_64 engine;
	return engine();
}

template<>
llong Random<llong>() {
	return static_cast<llong>(Random<ullong>());
}

template<>
uint Random<uint>(uint high) {
	return Random<uint>() % high;
}

template<>
int Random<int>(int high) {
	if (high <= 0) {
		throw InvalidArgumentException();
	}
	return Random<int>() % high;
}

template<>
ullong Random<ullong>(ullong high) {
	return Random<ullong>() % high;
}

template<>
llong Random<llong>(llong high) {
	if (high <= 0) {
		throw InvalidArgumentException();
	}
	return Random<llong>() % high;
}

template<>
float Random<float>(float high) {
	return (float)Random<uint>() / (float)UINT_MAX * high;
}

template<>
double Random<double>(double high) {
	return (double)Random<ullong>() / (double)ULLONG_MAX * high;
}

template<class T>
T Random<T>(T low, T high) {
	if (high <= low) {
		throw InvalidArgumentException();
	}
	return Random<T>(high - low) + low;
}

template uint Random<uint>(uint, uint);
template int Random<int>(int, int);
template ullong Random<ullong>(ullong, ullong);
template llong Random<llong>(llong, llong);
template float Random<float>(float, float);
template double Random<double>(double, double);