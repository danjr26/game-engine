#ifndef UNIFORM_FORCE_FIELD_H
#define UNIFORM_FORCE_FIELD_H

#include "force_field.h"
#include "collision_mask.h"
#include "collision_context.h"

template<uint n>
class UniformForceField : public ForceField<n> {
private:
	CollisionContext<double, n>* collisionContext;
	CollisionMask<double, n>* collisionMask;
	Vector<double, n> force;
public:
	UniformForceField(Vector<double, n> in_force, CollisionMask<double, n>* in_mask = nullptr, CollisionContext<double, n>* in_context = nullptr);
	~UniformForceField();
	void Set_Collision_Context(CollisionContext<double, n>* in_context) override;
	Vector<double, n> Calculate_Force(RigidBody<n>& in_rigidBody) override;
};

#endif


