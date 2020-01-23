#include "../include/internal/deep_transform.h"

template<class T, uint n>
Matrix<T, 4, 4> DeepTransform<T, n>::getLocalMatrix() const {
	return mDepth.getLocalMatrix() * mTransform.getLocalMatrix();
}

template<class T, uint n>
Matrix<T, 4, 4> DeepTransform<T, n>::getWorldMatrix() const {
	return mDepth.getWorldMatrix() * mTransform.getWorldMatrix();
}

template struct DeepTransform<float, 2>;
template struct DeepTransform<double, 2>;