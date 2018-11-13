#include "physics_manager.h"
#include "exceptions.h"
#include "game_engine.h"

PhysicsManager::PhysicsManager() :
	context(AxisAlignedRectangled::From_Extrema(Vector2d(0, 0), Vector2d(800, 600)), 2) {

	context.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::rigid_body), true);
}

void PhysicsManager::Update(double in_dt) {
	Exclude_Preembedded();

	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		(*it)->Update(in_dt);
	}


}

void PhysicsManager::Add(RigidBody2* in_rigidBody) {
	if (in_rigidBody == nullptr) {
		throw InvalidArgumentException();
	}
	rigidBody2s.push_back(in_rigidBody);
	in_rigidBody->Get_Collision_Mask()->Add_Filter(CollisionContext2d::rigid_body);
	context.Add(in_rigidBody->Get_Collision_Mask());
}

void PhysicsManager::Remove(RigidBody2* in_rigidBody) {
	auto position = std::find(rigidBody2s.begin(), rigidBody2s.end(), in_rigidBody);
	if (position != rigidBody2s.end()) {
		rigidBody2s.erase(position);
	}
	context.Remove(in_rigidBody->Get_Collision_Mask());
}

bool PhysicsManager::Has(RigidBody2* in_rigidBody) {
	return std::find(rigidBody2s.begin(), rigidBody2s.end(), in_rigidBody) != rigidBody2s.end();
}

void PhysicsManager::Exclude_Preembedded() {}

void PhysicsManager::Narrow_Collision_Interval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& in_collision, Ranged& in_range, uint in_nIterations) {
	// assume current rigid body state is at t = in_rangle.high
	Collision2d collision;
	InPlaceCollisionEvaluator2d collisionEvaluator;
	double t = in_range.Get_High();
	double dt = -in_range.Get_Span() / 2;
	for (uint i = 0; i < in_nIterations; i++) {
		in_body1.Update(dt);
		in_body2.Update(dt);
		t += dt;
		if ((collision = collisionEvaluator.Evaluate(*in_body1.Get_Collision_Mask(), *in_body2.Get_Collision_Mask())).didCollide) {
			in_range.Set_High(t);
			dt = -in_range.Get_Span() / 2;
			in_collision = collision;
		}
		else {
			in_range.Set_Low(t);
			dt = in_range.Get_Span() / 2;
		}
	}
}

/*
Vector2d PhysicsManager::Evaluate_Collision(RigidBody2& in_body1, RigidBody2& in_body2, const Vector2d& in_point) {
	
}
*/
	