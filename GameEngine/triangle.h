#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "n_gon.h"

template<class T, uint n>
class Triangle : public NGon<T, n, 3> {
	using NGon<T, n, 3>::NGon;
};

#endif