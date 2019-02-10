#ifndef PROJECTION_H
#define PROJECTION_H

#include "matrix.h"
#include "box.h"

template<class T>
class Projection {
public:
	Vector<T, 3> mMinima;
	Vector<T, 3> mMaxima;
	enum Type : bool {
		orthographic = false,
		perspective = true
	} type;

public:
	Projection();
	Projection(T in_near, T in_far, T in_horizontalAngle, T in_xToYRatio, Type in_type = Type::perspective);
	Projection(Vector<T, 3> in_minima, Vector<T, 3> in_maxima, Type in_type = Type::orthographic);

	T getViewAngle();
	void setViewAngle(T in_value);

	Matrix<T, 4, 4> getMatrix();
};

using Projectionf = Projection<float>;
using Projectiond = Projection<double>;

#endif