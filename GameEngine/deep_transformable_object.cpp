#include "deep_transformable_object.h"

template<class T, uint n>
DeepTransformableObject<T, n>::DeepTransformableObject(Transform<T, n>* in_transform, DepthTransform<T, n>* in_depthTransform) :
	TransformableObject<T, n>(in_transform),
	DeepObject<T, n>(in_depthTransform)
{}

template<class T, uint n>
Matrix<T, 4, 4> DeepTransformableObject<T, n>::getDeepLocalMatrix() const {
	return getDepthTransform().getLocalMatrix() * getTransform().getLocalMatrix();
}

template<class T, uint n>
Matrix<T, 4, 4> DeepTransformableObject<T, n>::getDeepWorldMatrix() const {
	return getDepthTransform().getWorldMatrix() * getTransform().getWorldMatrix();
}

template class DeepTransformableObject<float, 2>;
template class DeepTransformableObject<double, 2>;