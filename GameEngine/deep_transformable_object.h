#ifndef DEEP_TRANSFORMABLE_OBJECT_H
#define DEEP_TRANSFORMABLE_OBJECT_H

#include "deep_object.h"
#include "transformable_object.h"

template<class T, uint n>
class DeepTransformableObject : public TransformableObject<T, n>, public DeepObject<T, n> {
public:
	Matrix<T, 4, 4> getDeepLocalMatrix() const;
	Matrix<T, 4, 4> getDeepWorldMatrix() const;
};

using DeepTransformableObject2f = DeepTransformableObject<float, 2>;
using DeepTransformableObject2d = DeepTransformableObject<double, 2>;

#endif
