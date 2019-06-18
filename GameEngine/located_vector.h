#ifndef LOCATED_VECTOR_H
#define LOCATED_VECTOR_H

#include "vector.h"

template<class T, uint n>
struct LocatedVector {
	Vector<T, n> mPosition;
	Vector<T, n> mVector;

	LocatedVector() :
		mPosition(),
		mVector()
	{}

	LocatedVector(const Vector<T, n>& in_position, const Vector<T, n>& in_vector) :
		mPosition(in_position),
		mVector(in_vector)
	{}
};

using LocatedVector2f = LocatedVector<float, 2>;
using LocatedVector2d = LocatedVector<double, 2>;
using LocatedVector3f = LocatedVector<float, 3>;
using LocatedVector3d = LocatedVector<double, 3>;

#endif