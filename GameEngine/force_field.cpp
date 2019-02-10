#include "force_field.h"

template<uint n>
ForceField<n>::ForceField(CollisionMask<double, n>* in_mask) :
	mCollisionContext(nullptr),
	mCollisionMask(nullptr) {

	if (in_mask != nullptr) {
		mCollisionMask = in_mask->clone();
		mCollisionMask->addFilter(CollisionContext2d::force_field);
	}
}

template<uint n>
ForceField<n>::~ForceField() {
	if (mCollisionMask != nullptr) {
		delete mCollisionMask;
	}
}

template<uint n>
CollisionMask<double, n>* ForceField<n>::getCollisionMask() {
	return mCollisionMask;
}

template<uint n>
void ForceField<n>::setCollisionContext(CollisionContext<double, n>* in_context) {
	if (mCollisionContext != nullptr) {
		mCollisionContext->remove(mCollisionMask);
	}
	mCollisionContext = in_context;
	if (mCollisionMask != nullptr) {
		mCollisionContext->add(mCollisionMask);
	}
}


template class ForceField<2>;