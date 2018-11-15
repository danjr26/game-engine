#include "physics_manager.h"
#include "exceptions.h"
#include "game_engine.h"

PhysicsManager::PhysicsManager() :
	collisionContext(AxisAlignedRectangled::From_Extrema(Vector2d(0, 0), Vector2d(800, 600)), 2) {

	collisionContext.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::rigid_body), true);
	collisionContext.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::rigid_body_unstoppable), true);
	collisionContext.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::force_field), true);
}

void PhysicsManager::Update(double in_dt) {
	Exclude_Preembedded();

	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		(*it)->Update(in_dt);
	}

	collisionContext.Update();
	
	static int count = 0;
	count++;

	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		auto partners = collisionContext.Get_Partners((*it)->Get_Collision_Mask());
		for (auto jt = partners.first; jt < partners.second; jt++) {
			for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
				if ((*kt)->Get_Collision_Mask() == jt->mask) {
					if ((*kt)->Is_Unstoppable() || (*it)->Is_Unstoppable()) {
						Log::main(jt->collision.collisionPoint.To_String());
					}

					Ranged timeRange(0, in_dt);
					Collision2d collision = jt->collision;

					Narrow_Collision_Interval(**it, **kt, collision, timeRange, 3);

					Vector2d impulse = Evaluate_Collision(**it, **kt, collision);
					if (!(*it)->Is_Unstoppable()) {
						(*it)->Apply_World_Impulse(impulse, collision.collisionPoint);
					}

					if (!(*kt)->Is_Unstoppable()) {
						(*kt)->Apply_World_Impulse(-impulse, collision.collisionPoint);
					}

					(*it)->Update(in_dt - timeRange.Get_High());
					(*kt)->Update(in_dt - timeRange.Get_High());

					break;
				}
			}
		}
	}
}

void PhysicsManager::Add(RigidBody2* in_rigidBody) {
	if (in_rigidBody == nullptr) {
		throw InvalidArgumentException();
	}
	rigidBody2s.push_back(in_rigidBody);
	collisionContext.Add(in_rigidBody->Get_Collision_Mask());
}

void PhysicsManager::Add(ForceField2* in_forceField) {
	if (in_forceField == nullptr) {
		throw InvalidArgumentException();
	}
	forceField2s.push_back(in_forceField);
	in_forceField->Set_Collision_Context(&collisionContext);
}

void PhysicsManager::Remove(RigidBody2* in_rigidBody) {
	auto position = std::find(rigidBody2s.begin(), rigidBody2s.end(), in_rigidBody);
	if (position != rigidBody2s.end()) {
		rigidBody2s.erase(position);
	}
	collisionContext.Remove(in_rigidBody->Get_Collision_Mask());
}

bool PhysicsManager::Has(RigidBody2* in_rigidBody) {
	return std::find(rigidBody2s.begin(), rigidBody2s.end(), in_rigidBody) != rigidBody2s.end();
}

void PhysicsManager::Exclude_Preembedded() {}

void PhysicsManager::Narrow_Collision_Interval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& inout_collision, Ranged& inout_range, uint in_nIterations) {
	// assume current rigid body state is at t = in_range.high
	Collision2d collision;
	InPlaceCollisionEvaluator2d collisionEvaluator;
	double t = inout_range.Get_High();
	double dt = -inout_range.Get_Span() / 2;
	for (uint i = 0; i < in_nIterations; i++) {
		in_body1.Update(dt);
		in_body2.Update(dt);
		t += dt;
		if ((collision = collisionEvaluator.Evaluate(*in_body1.Get_Collision_Mask(), *in_body2.Get_Collision_Mask())).didCollide) {
			inout_range.Set_High(t);
			dt = -inout_range.Get_Span() / 2;
			inout_collision = collision;
		}
		else {
			inout_range.Set_Low(t);
			dt = inout_range.Get_Span() / 2;
		}
	}

	// leave it at new high
	in_body1.Update(inout_range.Get_High() - t);
	in_body2.Update(inout_range.Get_High() - t);
}

// returns impulse to apply to first body (reverse of impulse to apply to second body) 
Vector2d PhysicsManager::Evaluate_Collision(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision) {
	Vector2d point = in_collision.collisionPoint;
	Vector2d normal = in_body1.Get_Collision_Mask()->Get_Closest_Normal(point);
	Vector2d approachVelocity1 = in_body1.Get_World_Point_Velocity(point);
	Vector2d approachVelocity2 = in_body2.Get_World_Point_Velocity(point);
	Vector2d approachVelocity = approachVelocity2 - approachVelocity1;
	double restitution = Calculate_Restitution_Coefficient(in_body1, in_body2);
	double linearMass1 = in_body1.Get_Linear_Mass();
	double linearMass2 = in_body2.Get_Linear_Mass();
	double angularMass1 = in_body1.Get_Angular_Mass();
	double angularMass2 = in_body2.Get_Angular_Mass();
	Vector2d radius1 = point - in_body1.Get_Transform().Get_World_Position();
	Vector2d radius2 = point - in_body2.Get_Transform().Get_World_Position();

	if (approachVelocity.Dot(normal) >= 0) {
		return Vector2d(0, 0);
	}

	double linearFactor1 = (linearMass1 == 0.0) ? 0.0 : 1.0 / linearMass1;
	double linearFactor2 = (linearMass2 == 0.0) ? 0.0 : 1.0 / linearMass2;
	Vector2d angularFactor1 = (angularMass1 == 0.0) ? Vector2d(0, 0) : radius1.Orthogonal() * radius1.Magnitude() / angularMass1;
	Vector2d angularFactor2 = (angularMass2 == 0.0) ? Vector2d(0, 0) : radius2.Orthogonal() * radius2.Magnitude() / angularMass2;

	double numerator = (1.0 + restitution) * approachVelocity.Dot(normal);
	double denominator = (linearFactor1 + linearFactor2 + (angularFactor1 + angularFactor2).Dot(normal));

	return (denominator == 0.0) ? Vector2d(0, 0) : normal * (numerator / denominator);
}

double PhysicsManager::Calculate_Restitution_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.8;
}

double PhysicsManager::Calculate_Kinetic_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.5;
}

double PhysicsManager::Calculate_Static_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.6;
}


	