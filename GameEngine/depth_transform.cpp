#include "depth_transform.h"

template<class T, uint n>
inline DepthTransform<T, n>::DepthTransform() :
	parent(nullptr),
	depth(0) {}

template<class T, uint n>
inline DepthTransform<T, n>* DepthTransform<T, n>::Get_Parent() {
	return parent;
}

template<class T, uint n>
inline void DepthTransform<T, n>::Set_Parent(DepthTransform<T, n>* in_parent) {
	parent = in_parent;
}

template<class T, uint n>
inline T DepthTransform<T, n>::Get_Local_Depth() const {
	return depth;
}

template<class T, uint n>
inline T DepthTransform<T, n>::Get_World_Depth() const {
	T tempDepth = depth;
	for (DepthTransform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempDepth += transform->depth;
	}
	return tempDepth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::Set_Local_Depth(T in_depth) {
	depth = in_depth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::Set_World_Depth(T in_depth) {
	T tempDepth = in_depth;
	for (DepthTransform const* transform = parent; transform != nullptr; transform = transform->parent) {
		tempDepth -= transform->depth;
	}
	depth = tempDepth;
}

template<class T, uint n>
inline void DepthTransform<T, n>::Translate_Local_Depth(T in_depth) {
	Set_Local_Depth(depth + in_depth);
}

template<class T, uint n>
inline void DepthTransform<T, n>::Translate_World_Depth(T in_depth) {
	Set_World_Depth(Get_World_Depth() + in_depth);
}

template class DepthTransform<float, 2>;
template class DepthTransform<double, 2>;