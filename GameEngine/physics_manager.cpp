#include "physics_manager.h"
#include "exceptions.h"
#include "game_engine.h"
#include <unordered_set>
#include <unordered_map>

PhysicsManager::PhysicsManager() :
	collisionContext(AxisAlignedRectangled::From_Extrema(Vector2d(0, 0), Vector2d(800, 600)), 2),
	collisionNarrowSteps(3),
	maxStepRepeat(10),
	slidingEpsilon(2.0),
	restingEpsilon(1.0) {

	collisionContext.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::rigid_body), true);
	collisionContext.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::rigid_body_unstoppable), true);
	collisionContext.Set_Partner_Test_Activation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::force_field), true);
}

void PhysicsManager::Update(double in_dt) {
	Prepare_Data_Containers(in_dt);
	//Exclude_Preembedded(in_dt);

	collisionContext.Update();

	Step_All(in_dt);
	Apply_Force_Fields_To_Rigid_Bodies(in_dt);
	Apply_Rigid_Bodies_To_Rigid_Bodies(in_dt);
	//collisionContext.Update();
	//Calculate_Force_Field_Effects(in_dt);
	//Calculate_Rigid_Body_Effects(in_dt);
	//Apply_Effects_To_Rigid_Bodies(in_dt);
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

bool PhysicsManager::Has(ForceField2* in_forceField) {
	return std::find(forceField2s.begin(), forceField2s.end(), in_forceField) != forceField2s.end();
}

void PhysicsManager::Prepare_Data_Containers(double in_dt) {
	rigidBody2UpdateInfo.clear();
	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		rigidBody2UpdateInfo[*it] = RigidBodyUpdateInfo2();
		rigidBody2UpdateInfo[*it].rigidBodyInfo.finishTime = in_dt;
	}
}

void PhysicsManager::Exclude_Preembedded(double in_dt) {
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	std::vector<CollisionMask2d const*> tried;
	collisionContext.Update();
	for (uint i = 0; i < 5 && collisionContext.Get_Total_Partnerings() != 0; i++) {
		tried.clear();
		for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
			collisionContext.Get_Partners((*it)->Get_Collision_Mask(), partners);
			for (auto jt = partners.begin(); jt < partners.end(); jt++) {
				for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
					if ((*kt)->Get_Collision_Mask() == (*jt)->mask) {
						if (std::find(tried.begin(), tried.end(), (*kt)->Get_Collision_Mask()) != tried.end()) break;

						Collision2d collision = (*jt)->collision;
						Vector2d normal = RigidBody2::Get_Collision_Normal(**it, **kt, collision);

						tried.push_back((*kt)->Get_Collision_Mask());
						break;
					}
				}
			}
		}
		collisionContext.Update();
	}
}

void PhysicsManager::Step_All(double in_dt) {
	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		(*it)->Update(in_dt);
	}
}

void PhysicsManager::Apply_Rigid_Bodies_To_Rigid_Bodies(double in_dt) {
	std::vector<CollisionContext2d::CollisionPartner*> partners;

	double t = 0.0;

	Ranged minInterval(in_dt, in_dt);
	struct ProcessedPartnering {
		RigidBody2* body1;
		RigidBody2* body2;
		Transform2d transform1;
		Transform2d transform2;
		Collision2d collision;
	};
	std::vector<ProcessedPartnering> minPartners;

	collisionContext.Update();

	uint repeatCount = 0;
	while (collisionContext.Get_Total_Partnerings() > 0 && repeatCount < maxStepRepeat) {
		minPartners.clear();
		minInterval = Ranged(in_dt, in_dt);

		// find min partners
		for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
			RigidBody2* rigidBody1 = *it;
			collisionContext.Get_Partners(rigidBody1->Get_Collision_Mask(), partners);

			for (auto jt = partners.begin(); jt < partners.end(); jt++) {
				CollisionContext2d::CollisionPartner* partner = *jt;

				auto kt = std::find_if(rigidBody2s.begin(), rigidBody2s.end(),
					[partner](RigidBody2* rigidBody) {return rigidBody->Get_Collision_Mask() == partner->mask; });

				if (kt == rigidBody2s.end()) break;
				RigidBody2* rigidBody2 = *kt;


				if (std::find_if(minPartners.begin(), minPartners.end(),
					[rigidBody1, rigidBody2](const ProcessedPartnering& partnering) { 
						return partnering.body1 == rigidBody2 && partnering.body2 == rigidBody1;
				}) != minPartners.end()) break;

				ProcessedPartnering processed;
				processed.body1 = rigidBody1;
				processed.body2 = rigidBody2;
				processed.collision = partner->collision;

				int skipSteps = (int)(-log2((in_dt - t) / in_dt));
				int steps = Max(0, (int)collisionNarrowSteps - skipSteps);

				Ranged interval(t, in_dt);
				Narrow_Collision_Interval(
					*rigidBody1, *rigidBody2, 
					processed.collision, interval, steps, 
					&processed.transform1, &processed.transform2
				);

				if (interval.Get_Low() == t) {
					InPlaceCollisionEvaluator2d evaluator;
					evaluator.Return_Point(true);
					evaluator.Return_Separator(true);

					Transform2d transform1 = rigidBody1->Get_Transform();
					Transform2d transform2 = rigidBody2->Get_Transform();

					rigidBody1->Update(t - in_dt);
					rigidBody2->Update(t - in_dt);

					Collision2d collision = evaluator.Evaluate(*rigidBody1->Get_Collision_Mask(), *rigidBody2->Get_Collision_Mask());
					processed.collision.separator = collision.separator;
					processed.collision.owner = collision.owner;

					rigidBody1->Get_Transform() = transform1;
					rigidBody2->Get_Transform() = transform2;
				}

				if (processed.collision.owner == nullptr) {
					Log::main("err");
				}

				if (interval.Get_Low() < minInterval.Get_Low()) {
					minInterval = interval;
					minPartners.assign(1, processed);
				}
				else if (interval.Get_Low() == minInterval.Get_Low()) {
					minPartners.push_back(processed);
				}
			}
		}

		// apply impulses tp min partners
		std::unordered_map<RigidBody2*, Transform2d> uniqueBodies;
		for (auto it = minPartners.begin(); it != minPartners.end(); it++) {
			if (uniqueBodies.find(it->body1) == uniqueBodies.end()) {
				uniqueBodies.insert(std::pair<RigidBody2*, Transform2d>(it->body1, it->transform1));
				it->body1->Update(minInterval.Get_High() - in_dt);
			}

			if (uniqueBodies.find(it->body2) == uniqueBodies.end()) {
				uniqueBodies.insert(std::pair<RigidBody2*, Transform2d>(it->body2, it->transform2));
				it->body2->Update(minInterval.Get_High() - in_dt);
			}

			LocatedVector2d impulse;
			Vector2d friction;

			Evaluate_Collision(*it->body1, *it->body2, it->collision, impulse.vector, friction);

			impulse.position = it->collision.point;
			impulse.vector += friction;

			for (auto body : { it->body1, it->body2 }) {
				if (!body->Is_Unstoppable()) {
					body->Apply_World_Impulse(impulse);
				}
				impulse.vector = -impulse.vector;
			}

		}

		// return to end of frame with impulses applied
		for (auto it = uniqueBodies.begin(); it != uniqueBodies.end(); it++) {
			it->first->Get_Transform() = it->second;
		}

		for (auto it = uniqueBodies.begin(); it != uniqueBodies.end(); it++) {
			it->first->Update(in_dt - minInterval.Get_Low());
		}

		// update repeat tracker
		if (t >= minInterval.Get_Low()) {
			repeatCount++;
		}
		else {
			repeatCount = 0;
		}
		t = minInterval.Get_Low();

		collisionContext.Update();
	}

	/*
	if (repeatCount >= maxStepRepeat) {
		Log::main("unable to resolve");
		for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
			(*it)->Update(t - in_dt);
		}
	}
	*/
}

void PhysicsManager::Apply_Force_Fields_To_Rigid_Bodies(double in_dt) {
	collisionContext.Update();

	std::vector<CollisionContext2d::CollisionPartner*> partners;
	for (auto it = forceField2s.begin(); it < forceField2s.end(); it++) {
		ForceField2* forceField = *it;
		if (forceField->Get_Collision_Mask() == nullptr) {
			for (auto jt = rigidBody2s.begin(); jt < rigidBody2s.end(); jt++) {

				RigidBody2* rigidBody = *jt;
				if (rigidBody->Is_Unstoppable()) continue;

				LocatedVector2d impulse = forceField->Calculate_Force(*rigidBody);
				impulse.vector *= in_dt;

				rigidBody->Apply_World_Impulse(impulse);
			}
		}
		else {
			collisionContext.Get_Partners(forceField->Get_Collision_Mask(), partners);
			for (auto jt = partners.begin(); jt < partners.end(); jt++) {
				CollisionContext2d::CollisionPartner* partner = *jt;
				auto kt = std::find_if(rigidBody2s.begin(), rigidBody2s.end(), 
					[partner](RigidBody2* rigidBody) {return rigidBody->Get_Collision_Mask() == partner->mask; });

				if (kt == rigidBody2s.end()) continue;

				RigidBody2* rigidBody = *kt;
				if (rigidBody->Is_Unstoppable()) continue;

				LocatedVector2d impulse = forceField->Calculate_Force(*rigidBody);
				impulse.vector *= in_dt;

				rigidBody->Apply_World_Impulse(impulse);
			}
		}
	}
}

/*
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
					effectInfo.transform = (*jt)->Get_Transform();
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
							effectInfo.transform = (*kt)->Get_Transform();
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
			for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
				if ((*kt)->Get_Collision_Mask() == (*jt)->mask) {
					RigidBodyUpdateInfo2& info1 = rigidBody2UpdateInfo[*it];
					RigidBodyUpdateInfo2& info2 = rigidBody2UpdateInfo[*kt];

					if (info2.rigidBodyInfo.body == *it) break;

					Ranged interval(0, in_dt);
					Collision2d collision = (*jt)->collision;
					Narrow_Collision_Interval(**it, **kt, collision, interval, collisionNarrowSteps);

					LocatedVector2d impulse = { collision.point, Evaluate_Collision(**it, **kt, collision, interval.Get_High() - in_dt) };
					LocatedVector2d reverseImpulse = impulse;
					reverseImpulse.vector = -reverseImpulse.vector;
					Vector2d normal = RigidBody2::Get_Collision_Normal(**it, **kt, collision, interval.Get_High() - in_dt);

					if (!(*it)->Is_Unstoppable() && (info1.rigidBodyInfo.body == nullptr || info1.rigidBodyInfo.interval.Get_Low() > interval.Get_Low())) {
						EffectInfo2 effectInfo;
						(*it)->Update(interval.Get_High() - in_dt);
						effectInfo.transform = (*it)->Get_Transform();
						(*it)->Update(in_dt - interval.Get_High());
						effectInfo.body = *kt;
						effectInfo.impulse = impulse;
						effectInfo.normal = normal;
						effectInfo.interval = interval;

						info1.rigidBodyInfo = effectInfo;
					}

					if (!(*kt)->Is_Unstoppable() && (info2.rigidBodyInfo.body == nullptr || info2.rigidBodyInfo.interval.Get_Low() > interval.Get_Low())) {
						EffectInfo2 effectInfo;
						(*kt)->Update(interval.Get_High() - in_dt);
						effectInfo.transform = (*kt)->Get_Transform();
						(*kt)->Update(in_dt - interval.Get_High());
						effectInfo.body = *it;
						effectInfo.impulse = reverseImpulse;
						effectInfo.normal = normal;
						effectInfo.interval = interval;

						info2.rigidBodyInfo = effectInfo;
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
		if (updateInfo.rigidBodyInfo.body != nullptr) {
			(*it)->Update(updateInfo.rigidBodyInfo.interval.Get_Low() - in_dt);
		}
	}

	collisionContext.Update();
	if (collisionContext.Get_Total_Partnerings()) {
		Log::main("");
	}

	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		if (updateInfo.rigidBodyInfo.body != nullptr) {
			auto t = (*it)->Get_Transform();
			(*it)->Get_Transform() = updateInfo.rigidBodyInfo.transform;
			(*it)->Apply_World_Impulse(updateInfo.rigidBodyInfo.impulse);
			(*it)->Get_Transform() = t;
		}
	}

	/*
	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		//(*it)->Update(in_dt - updateInfo.rigidBodyInfo.interval.Get_Low());
	}

	collisionContext.Update();
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		collisionContext.Get_Partners((*it)->Get_Collision_Mask(), partners);
		for (auto jt = partners.begin(); jt < partners.end(); jt++) {
			for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
				RigidBodyUpdateInfo2& info1 = rigidBody2UpdateInfo[*it];
				RigidBodyUpdateInfo2& info2 = rigidBody2UpdateInfo[*kt];

				if (std::find(rigidBody2s.begin(), it, *kt) != it) break;

				Ranged interval(Min(info1.rigidBodyInfo.interval.Get_Low(), info2.rigidBodyInfo.interval.Get_Low()), in_dt);
				Collision2d collision = (*jt)->collision;
				Narrow_Collision_Interval(**it, **kt, collision, interval, collisionNarrowSteps);

				if (interval.Get_Low() < info1.rigidBodyInfo.finishTime) {
					info1.rigidBodyInfo.finishTime = interval.Get_Low();
				}

				if (interval.Get_Low() < info2.rigidBodyInfo.finishTime) {
					info2.rigidBodyInfo.finishTime = interval.Get_Low();
				}
			}
		}
	}

	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		if (updateInfo.rigidBodyInfo.finishTime < in_dt) {
			//(*it)->Update(Max(updateInfo.rigidBodyInfo.finishTime, updateInfo.rigidBodyInfo.interval.Get_Low()) - in_dt);
		}
	}
	


	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		for (auto jt = updateInfo.forceFieldInfo.begin(); jt != updateInfo.forceFieldInfo.end(); jt++) {
			auto t = (*it)->Get_Transform();
			(*it)->Get_Transform() = jt->transform;
			(*it)->Apply_World_Impulse(jt->impulse);
			(*it)->Get_Transform() = t;
		}
	}

	collisionContext.Update();
	if (collisionContext.Get_Total_Partnerings()) {
		Log::main("");
	}
}*/

void PhysicsManager::Narrow_Collision_Interval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& inout_collision, Ranged& inout_range, uint in_nIterations,
	Transform2d* out_transform1, Transform2d* out_transform2) {
	// assume current rigid body state is at t = in_range.high
	Collision2d collision; 

	InPlaceCollisionEvaluator2d collisionEvaluator;
	collisionEvaluator.Return_Point(true);
	collisionEvaluator.Return_Separator(true);

	Transform2d transform1 = in_body1.Get_Transform();
	Transform2d transform2 = in_body2.Get_Transform();

	if (out_transform1 != nullptr) {
		in_body1.Update(-inout_range.Get_Span());
		*out_transform1 = in_body1.Get_Transform();
		in_body1.Get_Transform() = transform1;
	}

	if (out_transform2 != nullptr) {
		in_body2.Update(-inout_range.Get_Span());
		*out_transform2 = in_body2.Get_Transform();
		in_body2.Get_Transform() = transform2;
	}

	double t = inout_range.Get_High();
	double dt = -inout_range.Get_Span() / 2;
	for (uint i = 0; i < in_nIterations; i++) {
		in_body1.Update(dt);
		in_body2.Update(dt);
		t += dt;
		collision = collisionEvaluator.Evaluate(*in_body1.Get_Collision_Mask(), *in_body2.Get_Collision_Mask());
		if (collision.did) {
			inout_range.Set_High(t);
			dt = -inout_range.Get_Span() / 2;
			inout_collision.did = collision.did;
			inout_collision.point = collision.point;
		}
		else {
			inout_range.Set_Low(t);
			dt = inout_range.Get_Span() / 2;
			inout_collision.separator = collision.separator;
			inout_collision.owner = collision.owner;
			if (out_transform1 != nullptr) *out_transform1 = in_body1.Get_Transform();
			if (out_transform2 != nullptr) *out_transform2 = in_body2.Get_Transform();
		}
	}

	in_body1.Get_Transform() = transform1;
	in_body2.Get_Transform() = transform2;
}

void PhysicsManager::Evaluate_Collision(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision, Vector2d& out_bounce, Vector2d& out_friction) {
	Vector2d point = in_collision.point;
	Vector2d normal = RigidBody2::Get_Collision_Normal(in_body1, in_body2, in_collision);

	double restitution = Calculate_Restitution_Coefficient(in_body1, in_body2);

	Vector2d approachVelocity1 = in_body1.Get_World_Point_Velocity(point);
	Vector2d approachVelocity2 = in_body2.Get_World_Point_Velocity(point);
	Vector2d approachVelocity = approachVelocity2 - approachVelocity1;

	double linearMass1 = in_body1.Get_Linear_Mass();
	double linearMass2 = in_body2.Get_Linear_Mass();

	double angularMass1 = in_body1.Get_Angular_Mass();
	double angularMass2 = in_body2.Get_Angular_Mass();

	Vector2d radius1 = point - in_body1.Get_Transform().Get_World_Position();
	Vector2d radius2 = point - in_body2.Get_Transform().Get_World_Position();

	if (approachVelocity.Dot(normal) >= 0) {
		normal = -normal;
	}

	double linearFactor1 = (linearMass1 == 0.0) ? 0.0 : 1.0 / linearMass1;
	double linearFactor2 = (linearMass2 == 0.0) ? 0.0 : 1.0 / linearMass2;

	Vector2d angularFactor1 = (angularMass1 == 0.0) ? Vector2d(0, 0) : radius1.Orthogonal() * radius1.Magnitude() / angularMass1;
	Vector2d angularFactor2 = (angularMass2 == 0.0) ? Vector2d(0, 0) : radius2.Orthogonal() * radius2.Magnitude() / angularMass2;
	Vector2d angularFactor = angularFactor1 + angularFactor2;

	double numerator = (1.0 + restitution) * approachVelocity.Dot(normal);
	double denominator = linearFactor1 + linearFactor2 + abs(angularFactor.Dot(normal));

	Vector2d impulse = (denominator == 0.0) ? Vector2d(0, 0) : normal * (-2.0 + (numerator / denominator));

	Vector2d parallelVelocity = approachVelocity - normal * approachVelocity.Dot(normal);
	double frictionCoeff = (parallelVelocity.Magnitude() <= slidingEpsilon) ?
		Calculate_Static_Friction_Coefficient(in_body1, in_body2) :
		Calculate_Kinetic_Friction_Coefficient(in_body1, in_body2);

	Vector2d friction = parallelVelocity.Normalized() * impulse.Magnitude() * frictionCoeff;
	Vector2d maxFriction = parallelVelocity / (linearFactor1 + linearFactor2 + abs(angularFactor.Dot(parallelVelocity.Normalized()))) * 0.99;

	if (friction.Dot_Self() > maxFriction.Dot_Self()) {
		friction = maxFriction;
	}

	out_bounce = impulse;
	out_friction = friction;
}

typename PhysicsManager::RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status PhysicsManager::Evaluate_Status(RigidBody2& in_body1, RigidBody2& in_body2, 
	const Collision2d& in_collision) {

	Vector2d point = in_collision.point;
	Vector2d normal = RigidBody2::Get_Collision_Normal(in_body1, in_body2, in_collision);
	Vector2d velocity1 = in_body1.Get_World_Point_Velocity(point);
	Vector2d velocity2 = in_body2.Get_World_Point_Velocity(point);
	Vector2d velocity = velocity2 - velocity1;
	Vector2d normalVelocity = velocity.Projection(normal);
	Vector2d tangentVelocity = velocity - normalVelocity;

	if (normalVelocity.Magnitude() <= slidingEpsilon) {
		if (tangentVelocity.Magnitude() <= restingEpsilon) {
			return RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status::resting;
		}
		else {
			return RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status::sliding;
		}
	}

	return RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status::bouncing;
}

void PhysicsManager::Conform_To_Status(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision, RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status in_status) {
	Vector2d point = in_collision.point;
	Vector2d velocity1 = in_body1.Get_World_Point_Velocity(point);
	Vector2d velocity2 = in_body2.Get_World_Point_Velocity(point);
}

double PhysicsManager::Calculate_Restitution_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.4;
}

double PhysicsManager::Calculate_Kinetic_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.5;
}

double PhysicsManager::Calculate_Static_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.6;
}


	