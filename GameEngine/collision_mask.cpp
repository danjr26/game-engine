#include "collision_mask.h"

template<class T>
CollisionMask<T, 2>::CollisionMask(bool in_ignoreTransform) :
	mIgnoreTransform(in_ignoreTransform),
	mParent(nullptr) {}

template<class T>
bool CollisionMask<T, 2>::getIgnoreTransform() const {
	return mIgnoreTransform;
}

template<class T>
void CollisionMask<T, 2>::setIgnoreTransform(bool in_value) {
	mIgnoreTransform = in_value;
}

template<class T>
void CollisionMask<T, 2>::setParent(void* in_parent) {
	mParent = in_parent;
}

template<class T>
void* CollisionMask<T, 2>::getParent() const {
	return mParent;
}

template class CollisionMask<float, 2>;
template class CollisionMask<double, 2>;