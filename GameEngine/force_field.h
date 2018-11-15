#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include <functional>
#include "rigid_body.h"
#include "collision_context.h"

template<uint n>
class ForceField {
public:
	virtual void Set_Collision_Context(CollisionContext<double, n>* in_context);
	virtual Vector<double, n> Calculate_Force(RigidBody<n>& in_rigidBody) = 0;
};

using ForceField2 = ForceField<2>;
using ForceField3 = ForceField<3>;

#endif


