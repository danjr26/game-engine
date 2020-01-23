#ifndef PROJECTION_H
#define PROJECTION_H

#include "../../../Geometry/include/geometry.h"

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
	Projection(T i_near, T i_far, T i_horizontalAngle, T i_xToYRatio, Type i_type = Type::perspective);
	Projection(Vector<T, 3> i_minima, Vector<T, 3> i_maxima, Type i_type = Type::orthographic);

	T getViewAngle();
	void setViewAngle(T i_value);

	Matrix<T, 4, 4> getMatrix();
};

using Projectionf = Projection<float>;
using Projectiond = Projection<double>;

#endif