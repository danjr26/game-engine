#include "collision_mask.h"

template<class T>
CollisionMask<T, 2>::CollisionMask(bool in_ignoreTransform) :
	mIgnoreTransform(in_ignoreTransform),
	mQueue(nullptr) {}

template<class T>
bool CollisionMask<T, 2>::getIgnoreTransform() const {
	return mIgnoreTransform;
}

template<class T>
void CollisionMask<T, 2>::setIgnoreTransform(bool in_value) {
	mIgnoreTransform = in_value;
}

template<class T>
void CollisionMask<T, 2>::setQueue(CollisionQueue<T, 2>* in_queue) {
	mQueue = in_queue;
}

template<class T>
CollisionQueue<T, 2>* CollisionMask<T, 2>::getQueue() {
	return mQueue;
}

template class CollisionMask<float, 2>;
template class CollisionMask<double, 2>;