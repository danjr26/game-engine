#ifndef BOX_H
#define BOX_H

#include "transform.h"
#include "definitions.h"

template<class T, uint n>
struct Box {
private:
	Vector<T, n> minima;
	Vector<T, n> maxima;

	Box(const Vector<T, n>& in_minima, const Vector<T, n>& in_maxima) :
		minima(in_minima),
		maxima(in_maxima)
	{}
};

using Rectanglei = Box<int, 2>;
using Rectanglef = Box<float, 2>;
using Rectangled = Box<double, 2>;

using Boxi = Box<int, 3>;
using Boxf = Box<float, 3>;
using Boxd = Box<double, 3>;

#endif