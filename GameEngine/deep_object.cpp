#include "deep_object.h"

template<class T, uint n>
inline DeepObject<T, n>::DeepObject() 
{}

template<class T, uint n>
inline DepthTransform<T, n>& DeepObject<T, n>::getDepthTransform() {
	return mDepthTransform;
}

template class DeepObject<float, 2>;
template class DeepObject<double, 2>;
