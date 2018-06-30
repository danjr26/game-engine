#ifndef N_SPHERE_H
#define N_SPHERE_H

#include "vector.h"

template<class T, uint n>
struct NSphere {
public:
	Vector<T, n> center;
	T radius;

	NSphere(Vector<T, n> in_center, T in_radius) :
		center(in_center),
		radius(in_radius)
	{}
};

using Circle2f = NSphere<float, 2>;
using Circle2d = NSphere<double, 2>;
using Spheref = NSphere<float, 3>;
using Sphered = NSphere<double, 3>;

#endif