#include "depth_transform.h"

template<class T, uint n>
inline DepthTransform<T, n>::DepthTransform() :
	mParent(nullptr),
	mDepth(0) {}

template<class T, uint n>
inline DepthTransform<T, n>* DepthTransform<T, n>::Get_Parent() {
	return mParent;
}

template<class T, uint n>
inline void DepthTransform<T, n>::Set_Parent(DepthTransform<T, n>* in_parent) {
	mParent = in_parent;
}

template<class T, uint n>
inline T DepthTransform<T, n>::Get_Local_Depth() const {
	return mDepth;
}

template<class T, uint n>
inline T DepthTransform<T, n>::Get_World_Depth() const {
	T tempDepth = mDepth;
	for (DepthTransform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDepth += transform->mDepth;
	}
	return tempDepth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::Set_Local_Depth(T in_depth) {
	mDepth = in_depth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::Set_World_Depth(T in_depth) {
	T tempDepth = in_depth;
	for (DepthTransform const* transform = mParent; transform != nullptr; transform = transform->mParent) {
		tempDepth -= transform->mDepth;
	}
	mDepth = tempDepth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::Translate_Local_Depth(T in_depth) {
	Set_Local_Depth(mDepth + in_depth);
}

template<class T, uint n>
inline void DepthTransform<T, n>::Translate_World_Depth(T in_depth) {
	Set_World_Depth(Get_World_Depth() + in_depth);
}

template class DepthTransform<float, 2>;
template class DepthTransform<double, 2>;