#ifndef UNIFORM_FORCE_FIELD_H
#define UNIFORM_FORCE_FIELD_H

#include "force_field.h"
#include "collider.h"
#include "collision_context.h"

template<uint n>
class UniformForceField : public ForceField<n> {
private:
	Vector<double, n> mForce;

public:
	UniformForceField(Vector<double, n> i_force, Collider<double, n>* i_mask = nullptr);
	LocatedVector<double, n> calculateForce(RigidBody<n>& i_rigidBody) override;
};

using UniformForceField2 = UniformForceField<2>;
using UniformForceField3 = UniformForceField<3>;

#endif


