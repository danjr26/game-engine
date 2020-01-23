#ifndef DEEP_TRANSFORM_H
#define DEEP_TRANSFORM_H

#include "transform.h"
#include "depth_transform.h"

template<class T, uint n>
struct DeepTransform {
	Transform<T, n> mTransform;
	DepthTransform<T, n> mDepth;

	Matrix<T, 4, 4> getLocalMatrix() const;
	Matrix<T, 4, 4> getWorldMatrix() const;
};

using DeepTransform2f = DeepTransform<float, 2>;
using DeepTransform2d = DeepTransform<double, 2>;

#endif
