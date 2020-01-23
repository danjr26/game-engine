#include "force_field.h"

template<uint n>
ForceField<n>::ForceField(Collider<double, n>* i_mask) :
	mCollisionContext(nullptr),
	mCollider(nullptr) {

	if (i_mask != nullptr) {
		mCollider = i_mask->clone();
		mCollider->addFilter(CollisionContext2d::force_field);
	}
}

template<uint n>
ForceField<n>::~ForceField() {
	if (mCollider != nullptr) {
		delete mCollider;
	}
}

template<uint n>
Collider<double, n>* ForceField<n>::getCollider() {
	return mCollider;
}

template<uint n>
void ForceField<n>::setCollisionContext(CollisionContext<double, n>* i_context) {
	if (mCollisionContext != nullptr) {
		mCollisionContext->remove(mCollider);
	}
	mCollisionContext = i_context;
	if (mCollider != nullptr) {
		mCollisionContext->add(mCollider);
	}
}


template class ForceField<2>;