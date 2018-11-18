#ifndef LOCATED_VECTOR_H
#define LOCATED_VECTOR_H

template<class T, uint n>
struct LocatedVector {
	Vector<T, n> position;
	Vector<T, n> vector;
};

using LocatedVector2f = LocatedVector<float, 2>;
using LocatedVector2d = LocatedVector<double, 2>;
using LocatedVector3f = LocatedVector<float, 3>;
using LocatedVector3d = LocatedVector<double, 3>;

#endif