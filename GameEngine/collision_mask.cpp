#include "collision_mask.h"

template<class T>
CollisionMask<T, 2>::CollisionMask(bool in_ignoreTransform) :
	ignoreTransform(in_ignoreTransform),
	parent(nullptr) {}

template<class T>
bool CollisionMask<T, 2>::Get_Ignore_Transform() const {
	return ignoreTransform;
}

template<class T>
void CollisionMask<T, 2>::Set_Ignore_Transform(bool in_value) {
	ignoreTransform = in_value;
}

template<class T>
void CollisionMask<T, 2>::Set_Parent(void* in_parent) {
	parent = in_parent;
}

template<class T>
void* CollisionMask<T, 2>::Get_Parent() const {
	return parent;
}

template class CollisionMask<float, 2>;
template class CollisionMask<double, 2>;