#ifndef UNIFORM_FORCE_FIELD_H
#define UNIFORM_FORCE_FIELD_H

#include "force_field.h"
#include "collision_mask.h"
#include "collision_context.h"

template<uint n>
class UniformForceField : public ForceField<n> {
private:
	Vector<double, n> force;

public:
	UniformForceField(Vector<double, n> in_force, CollisionMask<double, n>* in_mask = nullptr);
	LocatedVector<double, n> Calculate_Force(RigidBody<n>& in_rigidBody) override;
};

using UniformForceField2 = UniformForceField<2>;
using UniformForceField3 = UniformForceField<3>;

#endif


