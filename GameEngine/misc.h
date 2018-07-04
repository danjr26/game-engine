#ifndef MISC_H
#define MISC_H

#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <functional>
using namespace std;

template<class T>
T Clamp(T n, T floor, T ceiling) {
	if (n < floor)
		return floor;
	if (n > ceiling)
		return ceiling;
	return n;
}

template<class T>
T Clamp_Floor(T n, T floor) {
	if (n < floor)
		return floor;
	return n;
}

template<class T>
T Clamp_Ceiling(T n, T ceiling) {
	if (n > ceiling)
		return ceiling;
	return n;
}

template<class T>
T Mean(T a, T b) {
	return (a + b) * 0.5;
}

template<class T>
bool Is_Between_Exc(T a, T b1, T b2) {
	if (b1 < b2) {
		return a > b1 && a < b2;
	}
	else {
		return a > b2 && a < b1;
	}
}

template<class T>
bool Is_Between_Inc(T a, T b1, T b2) {
	if (b1 < b2) {
		return a >= b1 && a <= b2;
	}
	else {
		return a >= b2 && a <= b1;
	}
}

template<class T>
T Lesser_Of(T a, T b) {
	if (b < a) {
		return b;
	}
	else {
		return a;
	}
}

template<class T> 
T Greater_Of(T a, T b) {
	if (b > a) {
		return b;
	}
	else {
		return a;
	}
}

template<class T>
T Lerp(T a, T b, T t) {
	return a * (1 - t) + b * t;
}

template<class T>
T Inv_Lerp(T a, T b, T t) {
	return (t - a) / b;
}

template<class T>
T Sign(T n) {
	return (n > 0) ? 1 : ((n < 0) ? -1 : 0);
}

string Double_To_String(double d, int precision);

void Test_Function(void* arg);

#endif 

