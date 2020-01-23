#include "uniform_force_field.h"
#include "i_place_collision_evaluator.h"

template<uint n>
UniformForceField<n>::UniformForceField(Vector<double, n> i_force, Collider<double, n>* i_mask) :
	ForceField(i_mask),
	mForce(i_force) 
{}

template<uint n>
LocatedVector<double, n> UniformForceField<n>::calculateForce(RigidBody<n>& i_rigidBody) {
	return { i_rigidBody.getTransform().getWorldPosition(), mForce * i_rigidBody.getLinearMass() };
}

template class UniformForceField<2>;
