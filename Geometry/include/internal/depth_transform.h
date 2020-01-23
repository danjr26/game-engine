#ifndef DEPTH_TRANSFORM_H
#define DEPTH_TRANSFORM_H

#include "../../../Utilities/include/utilities.h"

template<class T, uint n>
class DepthTransform {
private:
	DepthTransform* mParent;
	T mDepth;

public:
	DepthTransform();

	DepthTransform<T, n>* getParent();
	void setParent(DepthTransform<T, n>* i_parent);

	T getLocal() const;
	T getWorld() const;
	void setLocal(T i_depth);
	void setWorld(T i_depth);
	void translateLocal(T i_depth);
	void translateWorld(T i_depth);
	Matrix<T, 4, 4> getLocalMatrix() const;
	Matrix<T, 4, 4> getWorldMatrix() const;

	DepthTransform<T, n>* cloneChain() const;
	void deleteChainParents();
};

using DepthTransform2f = DepthTransform<float, 2>;
using DepthTransform2d = DepthTransform<double, 2>;

#endif


