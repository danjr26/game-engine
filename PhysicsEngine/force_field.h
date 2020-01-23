#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include <functional>
#include "rigid_body.h"
#include "collision_context.h"

template<uint n>
class ForceField {
protected:
	CollisionContext<double, n>* mCollisionContext;
	Collider<double, n>* mCollider;

public:
	ForceField(Collider<double, n>* i_mask = nullptr);
	~ForceField();
	Collider<double, n>* getCollider();
	void setCollisionContext(CollisionContext<double, n>* i_context);
	virtual LocatedVector<double, n> calculateForce(RigidBody<n>& i_rigidBody) = 0;
};

using ForceField2 = ForceField<2>;
using ForceField3 = ForceField<3>;

#endif


