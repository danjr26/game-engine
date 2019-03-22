#ifndef DEEP_OBJECT_H
#define DEEP_OBJECT_H

#include "depth_transform.h"
#include "misc.h"

template<class T, uint n>
class DeepObject {
private:
	neuterable_ptr<DepthTransform<T, n>> mDepthTransform;

public:
	DeepObject();
	DeepObject(DepthTransform<T, n>* in_depthTransform);

	DepthTransform<T, n>& getDepthTransform();
	DepthTransform<T, n> const& getDepthTransform() const;
	void subDepthTransform(DepthTransform<T, n>* in_depthTransform);
};

using DeepObject2f = DeepObject<float, 2>;
using DeepObject2d = DeepObject<double, 2>;

#endif


