#include "../include/internal/depth_transform.h"

template<class T, uint n>
inline DepthTransform<T, n>::DepthTransform() :
	mParent(nullptr),
	mDepth(0) {}

template<class T, uint n>
inline DepthTransform<T, n>* DepthTransform<T, n>::getParent() {
	return mParent;
}

template<class T, uint n>
inline void DepthTransform<T, n>::setParent(DepthTransform<T, n>* i_parent) {
	mParent = i_parent;
}

template<class T, uint n>
inline T DepthTransform<T, n>::getLocal() const {
	return mDepth;
}

template<class T, uint n>
inline T DepthTransform<T, n>::getWorld() const {
	T tempDepth = mDepth;
	for (DepthTransform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDepth += transform->mDepth;
	}
	return tempDepth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::setLocal(T i_depth) {
	mDepth = i_depth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::setWorld(T i_depth) {
	T tempDepth = i_depth;
	for (DepthTransform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDepth -= transform->mDepth;
	}
	mDepth = tempDepth;
}

template<class T, uint n>
void DepthTransform<T, n>::translateLocal(T i_depth) {
	setLocal(mDepth + i_depth);
}

template<class T, uint n>
void DepthTransform<T, n>::translateWorld(T i_depth) {
	setWorld(getWorld() + i_depth);
}

template<class T, uint n>
Matrix<T, 4, 4> DepthTransform<T, n>::getLocalMatrix() const {
	return Matrix<T, 4, 4>::translation(Vector<T, 3>(0, 0, getLocal()));
}

template<class T, uint n>
Matrix<T, 4, 4> DepthTransform<T, n>::getWorldMatrix() const {
	return Matrix<T, 4, 4>::translation(Vector<T, 3>(0, 0, getWorld()));
}

template<class T, uint n>
DepthTransform<T, n>* DepthTransform<T, n>::cloneChain() const {
	DepthTransform<T, n>* out = new DepthTransform<T, n>(*this);
	for (DepthTransform<T, n>* it = out; it->mParent != nullptr; it = it->mParent) {
		it->mParent = new DepthTransform<T, n>(*it->mParent);
	}
	return out;
}

template<class T, uint n>
void DepthTransform<T, n>::deleteChainParents() {
	DepthTransform<T, n>* next;
	for (DepthTransform<T, n>* it = mParent; it != nullptr; it = next) {
		next = it->mParent;
		delete it;
		it = next;
	}
}

template class DepthTransform<float, 2>;
template class DepthTransform<double, 2>;