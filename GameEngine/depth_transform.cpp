#include "depth_transform.h"

template<class T, uint n>
inline DepthTransform<T, n>::DepthTransform() :
	mParent(nullptr),
	mDepth(0) {}

template<class T, uint n>
inline DepthTransform<T, n>* DepthTransform<T, n>::getParent() {
	return mParent;
}

template<class T, uint n>
inline void DepthTransform<T, n>::setParent(DepthTransform<T, n>* in_parent) {
	mParent = in_parent;
}

template<class T, uint n>
inline T DepthTransform<T, n>::getLocalDepth() const {
	return mDepth;
}

template<class T, uint n>
inline T DepthTransform<T, n>::getWorldDepth() const {
	T tempDepth = mDepth;
	for (DepthTransform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDepth += transform->mDepth;
	}
	return tempDepth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::setLocalDepth(T in_depth) {
	mDepth = in_depth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::setWorldDepth(T in_depth) {
	T tempDepth = in_depth;
	for (DepthTransform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDepth -= transform->mDepth;
	}
	mDepth = tempDepth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::translateLocalDepth(T in_depth) {
	setLocalDepth(mDepth + in_depth);
}

template<class T, uint n>
inline void DepthTransform<T, n>::translateWorldDepth(T in_depth) {
	setWorldDepth(getWorldDepth() + in_depth);
}

template<class T, uint n>
Matrix<T, 4, 4> DepthTransform<T, n>::getLocalMatrix() const {
	return Matrix<T, 4, 4>::translation(Vector<T, 3>(0, 0, getLocalDepth()));
}

template<class T, uint n>
Matrix<T, 4, 4> DepthTransform<T, n>::getWorldMatrix() const {
	return Matrix<T, 4, 4>::translation(Vector<T, 3>(0, 0, getWorldDepth()));
}

template class DepthTransform<float, 2>;
template class DepthTransform<double, 2>;