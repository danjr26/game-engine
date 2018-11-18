#include "uniform_force_field.h"
#include "in_place_collision_evaluator.h"

template<uint n>
UniformForceField<n>::UniformForceField(Vector<double, n> in_force, CollisionMask<double, n>* in_mask) :
	ForceField(in_mask),
	force(in_force) 
{}

template<uint n>
LocatedVector<double, n> UniformForceField<n>::Calculate_Force(RigidBody<n>& in_rigidBody) {
	return { in_rigidBody.Get_Transform().Get_World_Position(), force * in_rigidBody.Get_Linear_Mass() };
}

template class UniformForceField<2>;
