#include "force_field.h"

template<uint n>
ForceField<n>::ForceField(CollisionMask<double, n>* in_mask) :
	collisionContext(nullptr),
	collisionMask(nullptr) {

	if (in_mask != nullptr) {
		collisionMask = in_mask->Clone();
		collisionMask->Add_Filter(CollisionContext2d::force_field);
	}
}

template<uint n>
ForceField<n>::~ForceField() {
	if (collisionMask != nullptr) {
		delete collisionMask;
	}
}

template<uint n>
CollisionMask<double, n>* ForceField<n>::Get_Collision_Mask() {
	return collisionMask;
}

template<uint n>
void ForceField<n>::Set_Collision_Context(CollisionContext<double, n>* in_context) {
	if (collisionContext != nullptr) {
		collisionContext->Remove(collisionMask);
	}
	collisionContext = in_context;
	if (collisionMask != nullptr) {
		collisionContext->Add(collisionMask);
	}
}


template class ForceField<2>;