#include "uniform_force_field.h"
#include "in_place_collision_evaluator.h"

template<uint n>
UniformForceField<n>::UniformForceField(Vector<double, n> in_force, CollisionMask<double, n>* in_mask, CollisionContext<double, n>* in_context) :
	collisionContext(in_context),
	collisionMask(nullptr),
	force(in_force) {
	
	if (in_mask != nullptr) {
		collisionMask = in_mask->Clone();
		collisionMask->Add_Filter(CollisionContext2d::force_field);
		if (in_context != nullptr) {
			in_context->Add(collisionMask);
		}
	}
}

template<uint n>
UniformForceField<n>::~UniformForceField() {
	if (collisionMask != nullptr) {
		delete collisionMask;
	}
}

template<uint n>
void UniformForceField<n>::Set_Collision_Context(CollisionContext<double, n>* in_context) {
	if (collisionContext != nullptr) {
		collisionContext->Remove(collisionMask);
	}
	collisionContext = in_context;
	collisionContext->Add(collisionMask);
}

template<uint n>
Vector<double, n> UniformForceField<n>::Calculate_Force(RigidBody<n>& in_rigidBody) {
	if (collisionMask == nullptr) {
		return force;
	}
	else {
		if (collisionContext == nullptr) {
			InPlaceCollisionEvaluator<double, n> evaluator;
			evaluator.Return_Point(false);
			if (evaluator.Evaluate(*collisionMask, *in_rigidBody.Get_Collision_Mask()).didCollide) {
				return force;
			}
			else {
				return Vector<double, n>();
			}
		}
		else {
			auto search = collisionContext->Get_Partners(collisionMask);
			for (auto it = search.first; it < search.second; it++) {
				if (it->mask == in_rigidBody.Get_Collision_Mask()) {
					return force;
				}
			}
			return Vector<double, n>();
		}
	}
}

template class UniformForceField<2>;
