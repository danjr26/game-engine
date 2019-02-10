#ifndef DEEP_TRANSFORM_H
#define DEEP_TRANSFORM_H

#include "definitions.h"

template<class T, uint n>
class DepthTransform {
private:
	DepthTransform* mParent;
	T mDepth;

public:
	DepthTransform();

	DepthTransform<T, n>* getParent();
	void setParent(DepthTransform<T, n>* in_parent);

	T getLocalDepth() const;
	T getWorldDepth() const;
	void setLocalDepth(T in_depth);
	void setWorldDepth(T in_depth);
	void translateLocalDepth(T in_depth);
	void translateWorldDepth(T in_depth);
};

using DepthTransform2f = DepthTransform<float, 2>;
using DepthTransform2d = DepthTransform<double, 2>;

#endif


