#ifndef TRANSFORMABLE_OBJECT_H
#define TRANSFORMABLE_OBJECT_H

#include "transform.h"

template<class T, uint n>
class TransformableObject {
protected:
	Transform<T, n> mTransform;
public:
	TransformableObject();
	Transform<T, n>& Get_Transform();
};

using TransformableObject2f = TransformableObject<float, 2>;
using TransformableObject2d = TransformableObject<double, 2>;
using TransformableObject3f = TransformableObject<float, 3>;
using TransformableObject3d = TransformableObject<double, 3>;

#endif