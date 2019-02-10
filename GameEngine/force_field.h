#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include <functional>
#include "rigid_body.h"
#include "collision_context.h"

template<uint n>
class ForceField {
protected:
	CollisionContext<double, n>* mCollisionContext;
	CollisionMask<double, n>* mCollisionMask;

public:
	ForceField(CollisionMask<double, n>* in_mask = nullptr);
	~ForceField();
	CollisionMask<double, n>* getCollisionMask();
	void setCollisionContext(CollisionContext<double, n>* in_context);
	virtual LocatedVector<double, n> calculateForce(RigidBody<n>& in_rigidBody) = 0;
};

using ForceField2 = ForceField<2>;
using ForceField3 = ForceField<3>;

#endif


