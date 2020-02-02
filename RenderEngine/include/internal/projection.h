#ifndef PROJECTION_H
#define PROJECTION_H

#include "../../../Geometry/include/geometry.h"

enum class ProjectionType {
	undefined,
	orthographic,
	perspective
};

template<class T>
struct Projection {
	T mNear, mFar;
	Vector<T, 2> mNearDimen;
	ProjectionType mType;

public:
	Projection();

	Matrix<T, 4, 4> getMatrix() const;
};

using Projectionf = Projection<float>;
using Projectiond = Projection<double>;

#endif