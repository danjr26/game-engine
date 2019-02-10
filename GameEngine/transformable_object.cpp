#include "transformable_object.h"

template<class T, uint n>
TransformableObject<T, n>::TransformableObject()
{}

template<class T, uint n>
Transform<T, n>& TransformableObject<T, n>::getTransform() {
	return mTransform;
}

template class TransformableObject<float, 2>;
template class TransformableObject<double, 2>;
template class TransformableObject<float, 3>;
template class TransformableObject<double, 3>;
