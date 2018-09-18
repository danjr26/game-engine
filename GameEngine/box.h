#ifndef BOX_H
#define BOX_H

#include "transformable_object.h"
#include "definitions.h"
#include <initializer_list>

template<class T, uint n>
struct Box {
private:
	Vector<T, n> origin;
	Vector<T, n> axes[n];

private:
	Box(const Vector<T, n>& in_origin, std::initializer_list<const Vector<T, n>&> in_axes);

public:
	
};

using Rectanglef = Box<float, 2>;
using Rectangled = Box<double, 2>;
using Boxf = Box<float, 3>;
using Boxd = Box<double, 3>;

#endif