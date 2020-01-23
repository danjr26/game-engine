#include "../include/internal/misc.h"

template<>
bool random<bool>() {
	return random<uint>(2) == 1;
}

template<>
uint random<uint>() {
	static std::mt19937 engine;
	return engine();
}

template<>
int random<int>() {
	return static_cast<int>(random<uint>());
}

template<>
ullong random<ullong>() {
	static std::mt19937_64 engine;
	return engine();
}

template<>
llong random<llong>() {
	return static_cast<llong>(random<ullong>());
}

template<>
uint random<uint>(uint high) {
	return random<uint>() % high;
}

template<>
int random<int>(int high) {
	if (high <= 0) return 0;
	return random<int>() % high;
}

template<>
ullong random<ullong>(ullong high) {
	return random<ullong>() % high;
}

template<>
llong random<llong>(llong high) {
	if (high <= 0) return 0;
	return random<llong>() % high;
}

template<>
float random<float>(float high) {
	return (float)random<uint>() / (float)UINT_MAX * high;
}

template<>
double random<double>(double high) {
	return (double)random<ullong>() / (double)ULLONG_MAX * high;
}

template<class T>
T random<T>(T low, T high) {
	if (high <= low) return 0;
	return random<T>(high - low) + low;
}

template uint random<uint>(uint, uint);
template int random<int>(int, int);
template ullong random<ullong>(ullong, ullong);
template llong random<llong>(llong, llong);
template float random<float>(float, float);
template double random<double>(double, double);

