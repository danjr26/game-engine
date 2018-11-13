#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <vector>
#include "rigid_body.h"
#include "collision_context.h"
#include "range.h"

class PhysicsManager {
private:
	CollisionContext2d context;
	std::vector<RigidBody2*> rigidBody2s;

public:
	PhysicsManager();

	void Update(double in_dt);

	void Add(RigidBody2* in_rigidBody);
	void Remove(RigidBody2* in_rigidBody);
	bool Has(RigidBody2* in_rigidBody);

private:
	void Exclude_Preembedded();
	void Narrow_Collision_Interval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& in_collision, Ranged& in_range, uint in_nIterations);
	Vector2d Evaluate_Collision(RigidBody2& in_body1, RigidBody2& in_body2, const Vector2d& in_point);
};

#endif