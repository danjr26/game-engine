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

	DepthTransform<T, n>* Get_Parent();
	void Set_Parent(DepthTransform<T, n>* in_parent);

	T Get_Local_Depth() const;
	T Get_World_Depth() const;
	void Set_Local_Depth(T in_depth);
	void Set_World_Depth(T in_depth);
	void Translate_Local_Depth(T in_depth);
	void Translate_World_Depth(T in_depth);
};

using DepthTransform2f = DepthTransform<float, 2>;
using DepthTransform2d = DepthTransform<double, 2>;

#endif


