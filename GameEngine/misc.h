#ifndef  MISC_H
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

#define PI			3.141592653589793
#define E			2.718281828459045
#define DEG_TO_RAD	0.017453292519943
#define RAD_TO_DEG	59.29577951308233

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long int ulong;
typedef long double longd;
typedef ulong Flags;

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

string Double_To_String(double d, int precision);

//void Die(std::string error);

void Test_Function(void* arg);

#endif 

