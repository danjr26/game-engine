#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <vector>
#include "rigid_body.h"
#include "collision_context.h"
#include "range.h"
#include "force_field.h"

class PhysicsManager {
private:
	CollisionContext2d collisionContext;
	std::vector<RigidBody2*> rigidBody2s;
	std::vector<ForceField2*> forceField2s;

public:
	PhysicsManager();

	void Update(double in_dt);

	void Add(RigidBody2* in_rigidBody);
	void Add(ForceField2* in_forceField);
	void Remove(RigidBody2* in_rigidBody);
	void Remove(ForceField2* in_forceField);
	bool Has(RigidBody2* in_rigidBody);
	bool Has(ForceField2* in_forceField);

private:
	void Exclude_Preembedded();
	void Narrow_Collision_Interval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& inout_collision, Ranged& inout_range, uint in_nIterations);
	Vector2d Evaluate_Collision(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision);
	static double Calculate_Restitution_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
	static double Calculate_Kinetic_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
	static double Calculate_Static_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
};

#endif