#ifndef DEEP_OBJECT_H
#define DEEP_OBJECT_H

#include "depth_transform.h"

template<class T, uint n>
class DeepObject {
protected:
	DepthTransform<T, n> depthTransform;

public:
	DeepObject();
	DepthTransform<T, n>& Get_Depth_Transform();
};

using DeepObject2f = DeepObject<float, 2>;
using DeepObject2d = DeepObject<double, 2>;

#endif


