#include "uniform_force_field.h"
#include "in_place_collision_evaluator.h"

template<uint n>
UniformForceField<n>::UniformForceField(Vector<double, n> in_force, CollisionMask<double, n>* in_mask) :
	ForceField(in_mask),
	mForce(in_force) 
{}

template<uint n>
LocatedVector<double, n> UniformForceField<n>::calculateForce(RigidBody<n>& in_rigidBody) {
	return { in_rigidBody.getTransform().getWorldPosition(), mForce * in_rigidBody.getLinearMass() };
}

template class UniformForceField<2>;
