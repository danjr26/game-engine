#include "force_field.h"

template<uint n>
ForceField<n>::ForceField(CollisionMask<double, n>* in_mask) :
	mCollisionContext(nullptr),
	mCollisionMask(nullptr) {

	if (in_mask != nullptr) {
		mCollisionMask = in_mask->Clone();
		mCollisionMask->Add_Filter(CollisionContext2d::force_field);
	}
}

template<uint n>
ForceField<n>::~ForceField() {
	if (mCollisionMask != nullptr) {
		delete mCollisionMask;
	}
}

template<uint n>
CollisionMask<double, n>* ForceField<n>::Get_Collision_Mask() {
	return mCollisionMask;
}

template<uint n>
void ForceField<n>::Set_Collision_Context(CollisionContext<double, n>* in_context) {
	if (mCollisionContext != nullptr) {
		mCollisionContext->Remove(mCollisionMask);
	}
	mCollisionContext = in_context;
	if (mCollisionMask != nullptr) {
		mCollisionContext->Add(mCollisionMask);
	}
}


template class ForceField<2>;