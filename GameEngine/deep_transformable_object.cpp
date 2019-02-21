#include "deep_transformable_object.h"

template<class T, uint n>
Matrix<T, 4, 4> DeepTransformableObject<T, n>::getDeepLocalMatrix() const {
	return mDepthTransform.getLocalMatrix() * mTransform.getLocalMatrix();
}

template<class T, uint n>
Matrix<T, 4, 4> DeepTransformableObject<T, n>::getDeepWorldMatrix() const {
	return mDepthTransform.getWorldMatrix() * mTransform.getWorldMatrix();
}

template class DeepTransformableObject<float, 2>;
template class DeepTransformableObject<double, 2>;