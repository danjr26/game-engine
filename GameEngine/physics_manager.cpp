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
	Prepare_Data_Containers();
	Exclude_Preembedded();
	Step_All(in_dt);
	collisionContext.Update();
	Calculate_Force_Field_Effects(in_dt);
	Calculate_Rigid_Body_Effects(in_dt);
	Apply_Effects_To_Rigid_Bodies(in_dt);
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

void PhysicsManager::Remove(ForceField2* in_forceField) {
	auto position = std::find(forceField2s.begin(), forceField2s.end(), in_forceField);
	if (position != forceField2s.end()) {
		forceField2s.erase(position);
	}
	in_forceField->Set_Collision_Context(nullptr);
}

bool PhysicsManager::Has(RigidBody2* in_rigidBody) {
	return std::find(rigidBody2s.begin(), rigidBody2s.end(), in_rigidBody) != rigidBody2s.end();
}

bool PhysicsManager::Has(ForceField2 * in_forceField) {
	return std::find(forceField2s.begin(), forceField2s.end(), in_forceField) != forceField2s.end();
}

void PhysicsManager::Prepare_Data_Containers() {
	rigidBody2UpdateInfo.clear();
	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		rigidBody2UpdateInfo[*it] = RigidBodyUpdateInfo2();
	}
}

void PhysicsManager::Exclude_Preembedded() {
	
}

void PhysicsManager::Step_All(double in_dt) {
	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		(*it)->Update(in_dt);
	}
}

void PhysicsManager::Calculate_Force_Field_Effects(double in_dt) {
	using EffectInfo2 = typename RigidBodyUpdateInfo<2>::ForceFieldEffectInfo;
	std::vector<CollisionContext2d::CollisionPartner*> partners;

	for (auto it = forceField2s.begin(); it < forceField2s.end(); it++) {
		if ((*it)->Get_Collision_Mask() == nullptr) { 
			for (auto jt = rigidBody2s.begin(); jt < rigidBody2s.end(); jt++) {
				if (!(*jt)->Is_Unstoppable()) {

					LocatedVector2d impulse = (*it)->Calculate_Force(**jt);
					impulse.vector *= in_dt;

					EffectInfo2 effectInfo;
					effectInfo.field = *it;
					effectInfo.impulse = impulse;
					rigidBody2UpdateInfo[*jt].forceFieldInfo.push_back(effectInfo);
				}
			}
		}
		else {
			collisionContext.Get_Partners((*it)->Get_Collision_Mask(), partners);
			for (auto jt = partners.begin(); jt < partners.end(); jt++) {
				for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
					if ((*kt)->Get_Collision_Mask() == (*jt)->mask) {
						if (!(*kt)->Is_Unstoppable()) {

							LocatedVector2d impulse = (*it)->Calculate_Force(**kt);
							impulse.vector *= in_dt;

							EffectInfo2 effectInfo;
							effectInfo.field = *it;
							effectInfo.impulse = impulse;
							rigidBody2UpdateInfo[*kt].forceFieldInfo.push_back(effectInfo);
						}

						break;
					}
				}
			}
		}
	}
}

void PhysicsManager::Calculate_Rigid_Body_Effects(double in_dt) {
	using EffectInfo2 = typename RigidBodyUpdateInfo<2>::RigidBodyEffectInfo;
	using Status2 = typename EffectInfo2::Status;
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		collisionContext.Get_Partners((*it)->Get_Collision_Mask(), partners);
		for (auto jt = partners.begin(); jt < partners.end(); jt++) {
			// check if it's a rigid body
			for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
				if ((*kt)->Get_Collision_Mask() == (*jt)->mask) {
					RigidBodyUpdateInfo2& info1 = rigidBody2UpdateInfo[*it];
					RigidBodyUpdateInfo2& info2 = rigidBody2UpdateInfo[*kt];
					if (std::find_if(info2.rigidBodyInfo.begin(), info2.rigidBodyInfo.end(), 
						[=](typename RigidBodyUpdateInfo2::RigidBodyEffectInfo& r) { return r.body == *it; }) != info2.rigidBodyInfo.end()) {
						break;
					}

					Ranged timeRange(0, in_dt);

					Collision2d collision = (*jt)->collision;

					Narrow_Collision_Interval(**it, **kt, collision, timeRange, collisionNarrowSteps);

					LocatedVector2d impulse = { collision.collisionPoint, Evaluate_Collision(**it, **kt, collision) };

					if (!(*it)->Is_Unstoppable()) {
						EffectInfo2 effectInfo;
						effectInfo.body = *kt;
						effectInfo.impulse = impulse;
						effectInfo.precollisionTime = timeRange.Get_Low();

						info1.rigidBodyInfo.push_back(effectInfo);
					}

					if (!(*kt)->Is_Unstoppable()) {
						EffectInfo2 effectInfo;
						effectInfo.body = *it;
						effectInfo.impulse = impulse;
						effectInfo.precollisionTime = timeRange.Get_Low();

						info1.rigidBodyInfo.push_back(effectInfo);
					}

					break;
				}
			}
		}
	}
}

void PhysicsManager::Apply_Effects_To_Rigid_Bodies(double in_dt) {
	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];

		double minTime = in_dt;
		for (auto jt = updateInfo.rigidBodyInfo.begin(); jt != updateInfo.rigidBodyInfo.end(); jt++) {
			if (jt->precollisionTime < minTime) {
				minTime = jt->precollisionTime;
			}
		}

		if (minTime < in_dt) {
			(*it)->Update(minTime - in_dt);
		}

		for (auto jt = updateInfo.forceFieldInfo.begin(); jt != updateInfo.forceFieldInfo.end(); jt++) {
			(*it)->Apply_World_Impulse(jt->impulse);
		}

		for (auto jt = updateInfo.rigidBodyInfo.begin(); jt != updateInfo.rigidBodyInfo.end(); jt++) {
			(*it)->Apply_World_Impulse(jt->impulse);
		}
	}
}

void PhysicsManager::Narrow_Collision_Interval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& inout_collision, Ranged& inout_range, uint in_nIterations) {
	// assume current rigid body state is at t = in_range.high
	Collision2d collision;
	InPlaceCollisionEvaluator2d collisionEvaluator;
	double returnT = inout_range.Get_High();
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

	// leave it as you found it
	in_body1.Update(returnT - t);
	in_body2.Update(returnT - t);
}

void PhysicsManager::Narrow_Collision_Interval(RigidBody2& in_body, RigidBody2*& out_otherBody, Collision2d& out_collision, Ranged& inout_range, uint in_nIterations) {
	
}

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
	Vector2d angularFactor = angularFactor1 + angularFactor2;

	double numerator = (1.0 + restitution) * approachVelocity.Dot(normal);
	double denominator = (linearFactor1 + linearFactor2 + angularFactor.Dot(normal));

	return (denominator == 0.0) ? Vector2d(0, 0) : normal * (numerator / denominator);
}

double PhysicsManager::Calculate_Restitution_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.5;
}

double PhysicsManager::Calculate_Kinetic_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.5;
}

double PhysicsManager::Calculate_Static_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.6;
}


	