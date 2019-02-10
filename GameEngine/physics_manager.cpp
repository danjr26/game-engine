#include "physics_manager.h"
#include "exceptions.h"
#include "log.h"
#include <unordered_set>
#include <unordered_map>

PhysicsManager::PhysicsManager() :
	collisionContext(AxisAlignedRectangled::fromExtrema(Vector2d(0, 0), Vector2d(800, 600)), 2),
	collisionNarrowSteps(3),
	maxStepRepeat(10),
	slidingEpsilon(2.0),
	restingEpsilon(1.0) {

	collisionContext.setPartnerTestActivation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::rigid_body), true);
	collisionContext.setPartnerTestActivation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::rigid_body_unstoppable), true);
	collisionContext.setPartnerTestActivation(std::pair<ubyte, ubyte>(CollisionContext2d::rigid_body, CollisionContext2d::force_field), true);
}

void PhysicsManager::update(double in_dt) {
	prepareDataContainers(in_dt);
	//excludePreembedded(in_dt);

	collisionContext.update();

	stepAll(in_dt);
	applyForceFieldsToRigidBodies(in_dt);
	applyRigidBodiesToRigidBodies(in_dt);
	//collisionContext.update();
	//calculateForceFieldEffects(in_dt);
	//calculateRigidBodyEffects(in_dt);
	//applyEffectsToRigidBodies(in_dt);
}

void PhysicsManager::add(RigidBody2* in_rigidBody) {
	if (in_rigidBody == nullptr) {
		throw InvalidArgumentException();
	}
	rigidBody2s.push_back(in_rigidBody);
	collisionContext.add(in_rigidBody->getCollisionMask());
}

void PhysicsManager::add(ForceField2* in_forceField) {
	if (in_forceField == nullptr) {
		throw InvalidArgumentException();
	}
	forceField2s.push_back(in_forceField);
	in_forceField->setCollisionContext(&collisionContext);
}

void PhysicsManager::remove(RigidBody2* in_rigidBody) {
	auto position = std::find(rigidBody2s.begin(), rigidBody2s.end(), in_rigidBody);
	if (position != rigidBody2s.end()) {
		rigidBody2s.erase(position);
	}
	collisionContext.remove(in_rigidBody->getCollisionMask());
}

void PhysicsManager::remove(ForceField2* in_forceField) {
	auto position = std::find(forceField2s.begin(), forceField2s.end(), in_forceField);
	if (position != forceField2s.end()) {
		forceField2s.erase(position);
	}
	in_forceField->setCollisionContext(nullptr);
}

bool PhysicsManager::has(RigidBody2* in_rigidBody) {
	return std::find(rigidBody2s.begin(), rigidBody2s.end(), in_rigidBody) != rigidBody2s.end();
}

bool PhysicsManager::has(ForceField2* in_forceField) {
	return std::find(forceField2s.begin(), forceField2s.end(), in_forceField) != forceField2s.end();
}

void PhysicsManager::prepareDataContainers(double in_dt) {
	rigidBody2UpdateInfo.clear();
	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		rigidBody2UpdateInfo[*it] = RigidBodyUpdateInfo2();
		rigidBody2UpdateInfo[*it].rigidBodyInfo.finishTime = in_dt;
	}
}

void PhysicsManager::excludePreembedded(double in_dt) {
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	std::vector<CollisionMask2d const*> tried;
	collisionContext.update();
	for (uint i = 0; i < 5 && collisionContext.getTotalPartnerings() != 0; i++) {
		tried.clear();
		for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
			collisionContext.getPartners((*it)->getCollisionMask(), partners);
			for (auto jt = partners.begin(); jt < partners.end(); jt++) {
				for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
					if ((*kt)->getCollisionMask() == (*jt)->mMask) {
						if (std::find(tried.begin(), tried.end(), (*kt)->getCollisionMask()) != tried.end()) break;

						Collision2d collision = (*jt)->mCollision;
						Vector2d normal = RigidBody2::getCollisionNormal(**it, **kt, collision);

						tried.push_back((*kt)->getCollisionMask());
						break;
					}
				}
			}
		}
		collisionContext.update();
	}
}

void PhysicsManager::stepAll(double in_dt) {
	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		(*it)->update(in_dt);
	}
}

void PhysicsManager::applyRigidBodiesToRigidBodies(double in_dt) {
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

	collisionContext.update();

	uint repeatCount = 0;
	while (collisionContext.getTotalPartnerings() > 0 && repeatCount < maxStepRepeat) {
		minPartners.clear();
		minInterval = Ranged(in_dt, in_dt);

		// find min partners
		for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
			RigidBody2* rigidBody1 = *it;
			collisionContext.getPartners(rigidBody1->getCollisionMask(), partners);

			for (auto jt = partners.begin(); jt < partners.end(); jt++) {
				CollisionContext2d::CollisionPartner* partner = *jt;

				auto kt = std::find_if(rigidBody2s.begin(), rigidBody2s.end(),
					[partner](RigidBody2* rigidBody) {return rigidBody->getCollisionMask() == partner->mMask; });

				if (kt == rigidBody2s.end()) break;
				RigidBody2* rigidBody2 = *kt;


				if (std::find_if(minPartners.begin(), minPartners.end(),
					[rigidBody1, rigidBody2](const ProcessedPartnering& partnering) { 
						return partnering.body1 == rigidBody2 && partnering.body2 == rigidBody1;
				}) != minPartners.end()) break;

				ProcessedPartnering processed;
				processed.body1 = rigidBody1;
				processed.body2 = rigidBody2;
				processed.collision = partner->mCollision;

				int skipSteps = (int)(-log2((in_dt - t) / in_dt));
				int steps = GEUtil::max(0, (int)collisionNarrowSteps - skipSteps);

				Ranged interval(t, in_dt);
				narrowCollisionInterval(
					*rigidBody1, *rigidBody2, 
					processed.collision, interval, steps, 
					&processed.transform1, &processed.transform2
				);

				if (interval.getLow() == t) {
					InPlaceCollisionEvaluator2d evaluator;
					evaluator.returnPoint(true);
					evaluator.returnSeparator(true);

					Transform2d transform1 = rigidBody1->getTransform();
					Transform2d transform2 = rigidBody2->getTransform();

					rigidBody1->update(t - in_dt);
					rigidBody2->update(t - in_dt);

					Collision2d collision = evaluator.evaluate(*rigidBody1->getCollisionMask(), *rigidBody2->getCollisionMask());
					processed.collision.mSeparator = collision.mSeparator;
					processed.collision.mOwner = collision.mOwner;

					rigidBody1->getTransform() = transform1;
					rigidBody2->getTransform() = transform2;
				}

				if (processed.collision.mOwner == nullptr) {
					Log::main("err");
				}

				if (interval.getLow() < minInterval.getLow()) {
					minInterval = interval;
					minPartners.assign(1, processed);
				}
				else if (interval.getLow() == minInterval.getLow()) {
					minPartners.push_back(processed);
				}
			}
		}

		// apply impulses tp min partners
		std::unordered_map<RigidBody2*, Transform2d> uniqueBodies;
		for (auto it = minPartners.begin(); it != minPartners.end(); it++) {
			if (uniqueBodies.find(it->body1) == uniqueBodies.end()) {
				uniqueBodies.insert(std::pair<RigidBody2*, Transform2d>(it->body1, it->transform1));
				it->body1->update(minInterval.getHigh() - in_dt);
			}

			if (uniqueBodies.find(it->body2) == uniqueBodies.end()) {
				uniqueBodies.insert(std::pair<RigidBody2*, Transform2d>(it->body2, it->transform2));
				it->body2->update(minInterval.getHigh() - in_dt);
			}

			LocatedVector2d impulse;
			Vector2d friction;

			evaluateCollision(*it->body1, *it->body2, it->collision, impulse.mVector, friction);

			impulse.mPosition = it->collision.mPoint;
			impulse.mVector += friction;

			for (auto body : { it->body1, it->body2 }) {
				if (!body->isUnstoppable()) {
					body->applyWorldImpulse(impulse);
				}
				impulse.mVector = -impulse.mVector;
			}

		}

		// return to end of frame with impulses applied
		for (auto it = uniqueBodies.begin(); it != uniqueBodies.end(); it++) {
			it->first->getTransform() = it->second;
		}

		for (auto it = uniqueBodies.begin(); it != uniqueBodies.end(); it++) {
			it->first->update(in_dt - minInterval.getLow());
		}

		// update repeat tracker
		if (t >= minInterval.getLow()) {
			repeatCount++;
		}
		else {
			repeatCount = 0;
		}
		t = minInterval.getLow();

		collisionContext.update();
	}

	/*
	if (repeatCount >= maxStepRepeat) {
		Log::main("unable to resolve");
		for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
			(*it)->update(t - in_dt);
		}
	}
	*/
}

void PhysicsManager::applyForceFieldsToRigidBodies(double in_dt) {
	collisionContext.update();

	std::vector<CollisionContext2d::CollisionPartner*> partners;
	for (auto it = forceField2s.begin(); it < forceField2s.end(); it++) {
		ForceField2* forceField = *it;
		if (forceField->getCollisionMask() == nullptr) {
			for (auto jt = rigidBody2s.begin(); jt < rigidBody2s.end(); jt++) {

				RigidBody2* rigidBody = *jt;
				if (rigidBody->isUnstoppable()) continue;

				LocatedVector2d impulse = forceField->calculateForce(*rigidBody);
				impulse.mVector *= in_dt;

				rigidBody->applyWorldImpulse(impulse);
			}
		}
		else {
			collisionContext.getPartners(forceField->getCollisionMask(), partners);
			for (auto jt = partners.begin(); jt < partners.end(); jt++) {
				CollisionContext2d::CollisionPartner* partner = *jt;
				auto kt = std::find_if(rigidBody2s.begin(), rigidBody2s.end(), 
					[partner](RigidBody2* rigidBody) {return rigidBody->getCollisionMask() == partner->mMask; });

				if (kt == rigidBody2s.end()) continue;

				RigidBody2* rigidBody = *kt;
				if (rigidBody->isUnstoppable()) continue;

				LocatedVector2d impulse = forceField->calculateForce(*rigidBody);
				impulse.mVector *= in_dt;

				rigidBody->applyWorldImpulse(impulse);
			}
		}
	}
}

/*
void PhysicsManager::calculateForceFieldEffects(double in_dt) {
	using EffectInfo2 = typename RigidBodyUpdateInfo<2>::ForceFieldEffectInfo;
	std::vector<CollisionContext2d::CollisionPartner*> partners;

	for (auto it = forceField2s.begin(); it < forceField2s.end(); it++) {
		if ((*it)->getCollisionMask() == nullptr) { 
			for (auto jt = rigidBody2s.begin(); jt < rigidBody2s.end(); jt++) {
				if (!(*jt)->isUnstoppable()) {

					LocatedVector2d impulse = (*it)->calculateForce(**jt);
					impulse.mVector *= in_dt;

					EffectInfo2 effectInfo;
					effectInfo.transform = (*jt)->getTransform();
					effectInfo.field = *it;
					effectInfo.impulse = impulse;
					rigidBody2UpdateInfo[*jt].forceFieldInfo.push_back(effectInfo);
				}
			}
		}
		else {
			collisionContext.getPartners((*it)->getCollisionMask(), partners);
			for (auto jt = partners.begin(); jt < partners.end(); jt++) {
				for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
					if ((*kt)->getCollisionMask() == (*jt)->mask) {
						if (!(*kt)->isUnstoppable()) {

							LocatedVector2d impulse = (*it)->calculateForce(**kt);
							impulse.mVector *= in_dt;

							EffectInfo2 effectInfo;
							effectInfo.transform = (*kt)->getTransform();
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

void PhysicsManager::calculateRigidBodyEffects(double in_dt) {
	using EffectInfo2 = typename RigidBodyUpdateInfo<2>::RigidBodyEffectInfo;
	using Status2 = typename EffectInfo2::Status;
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		collisionContext.getPartners((*it)->getCollisionMask(), partners);
		for (auto jt = partners.begin(); jt < partners.end(); jt++) {
			for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
				if ((*kt)->getCollisionMask() == (*jt)->mask) {
					RigidBodyUpdateInfo2& info1 = rigidBody2UpdateInfo[*it];
					RigidBodyUpdateInfo2& info2 = rigidBody2UpdateInfo[*kt];

					if (info2.rigidBodyInfo.body == *it) break;

					Ranged interval(0, in_dt);
					Collision2d collision = (*jt)->collision;
					narrowCollisionInterval(**it, **kt, collision, interval, collisionNarrowSteps);

					LocatedVector2d impulse = { collision.mPoint, evaluateCollision(**it, **kt, collision, interval.getHigh() - in_dt) };
					LocatedVector2d reverseImpulse = impulse;
					reverseImpulse.vector = -reverseImpulse.vector;
					Vector2d normal = RigidBody2::getCollisionNormal(**it, **kt, collision, interval.getHigh() - in_dt);

					if (!(*it)->isUnstoppable() && (info1.rigidBodyInfo.body == nullptr || info1.rigidBodyInfo.interval.getLow() > interval.getLow())) {
						EffectInfo2 effectInfo;
						(*it)->update(interval.getHigh() - in_dt);
						effectInfo.transform = (*it)->getTransform();
						(*it)->update(in_dt - interval.getHigh());
						effectInfo.body = *kt;
						effectInfo.impulse = impulse;
						effectInfo.normal = normal;
						effectInfo.interval = interval;

						info1.rigidBodyInfo = effectInfo;
					}

					if (!(*kt)->isUnstoppable() && (info2.rigidBodyInfo.body == nullptr || info2.rigidBodyInfo.interval.getLow() > interval.getLow())) {
						EffectInfo2 effectInfo;
						(*kt)->update(interval.getHigh() - in_dt);
						effectInfo.transform = (*kt)->getTransform();
						(*kt)->update(in_dt - interval.getHigh());
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

void PhysicsManager::applyEffectsToRigidBodies(double in_dt) {
	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		if (updateInfo.rigidBodyInfo.body != nullptr) {
			(*it)->update(updateInfo.rigidBodyInfo.interval.getLow() - in_dt);
		}
	}

	collisionContext.update();
	if (collisionContext.getTotalPartnerings()) {
		Log::main("");
	}

	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		if (updateInfo.rigidBodyInfo.body != nullptr) {
			auto t = (*it)->getTransform();
			(*it)->getTransform() = updateInfo.rigidBodyInfo.transform;
			(*it)->applyWorldImpulse(updateInfo.rigidBodyInfo.impulse);
			(*it)->getTransform() = t;
		}
	}

	/*
	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		//(*it)->update(in_dt - updateInfo.rigidBodyInfo.interval.getLow());
	}

	collisionContext.update();
	std::vector<CollisionContext2d::CollisionPartner*> partners;
	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		collisionContext.getPartners((*it)->getCollisionMask(), partners);
		for (auto jt = partners.begin(); jt < partners.end(); jt++) {
			for (auto kt = rigidBody2s.begin(); kt < rigidBody2s.end(); kt++) {
				RigidBodyUpdateInfo2& info1 = rigidBody2UpdateInfo[*it];
				RigidBodyUpdateInfo2& info2 = rigidBody2UpdateInfo[*kt];

				if (std::find(rigidBody2s.begin(), it, *kt) != it) break;

				Ranged interval(min(info1.rigidBodyInfo.interval.getLow(), info2.rigidBodyInfo.interval.getLow()), in_dt);
				Collision2d collision = (*jt)->collision;
				narrowCollisionInterval(**it, **kt, collision, interval, collisionNarrowSteps);

				if (interval.getLow() < info1.rigidBodyInfo.finishTime) {
					info1.rigidBodyInfo.finishTime = interval.getLow();
				}

				if (interval.getLow() < info2.rigidBodyInfo.finishTime) {
					info2.rigidBodyInfo.finishTime = interval.getLow();
				}
			}
		}
	}

	for (auto it = rigidBody2s.begin(); it < rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		if (updateInfo.rigidBodyInfo.finishTime < in_dt) {
			//(*it)->update(max(updateInfo.rigidBodyInfo.finishTime, updateInfo.rigidBodyInfo.interval.getLow()) - in_dt);
		}
	}
	


	for (auto it = rigidBody2s.begin(); it != rigidBody2s.end(); it++) {
		RigidBodyUpdateInfo2& updateInfo = rigidBody2UpdateInfo[*it];
		for (auto jt = updateInfo.forceFieldInfo.begin(); jt != updateInfo.forceFieldInfo.end(); jt++) {
			auto t = (*it)->getTransform();
			(*it)->getTransform() = jt->transform;
			(*it)->applyWorldImpulse(jt->impulse);
			(*it)->getTransform() = t;
		}
	}

	collisionContext.update();
	if (collisionContext.getTotalPartnerings()) {
		Log::main("");
	}
}*/

void PhysicsManager::narrowCollisionInterval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& inout_collision, Ranged& inout_range, uint in_nIterations,
	Transform2d* out_transform1, Transform2d* out_transform2) {
	// assume current rigid body state is at t = in_range.high
	Collision2d collision; 

	InPlaceCollisionEvaluator2d collisionEvaluator;
	collisionEvaluator.returnPoint(true);
	collisionEvaluator.returnSeparator(true);

	Transform2d transform1 = in_body1.getTransform();
	Transform2d transform2 = in_body2.getTransform();

	if (out_transform1 != nullptr) {
		in_body1.update(-inout_range.getSpan());
		*out_transform1 = in_body1.getTransform();
		in_body1.getTransform() = transform1;
	}

	if (out_transform2 != nullptr) {
		in_body2.update(-inout_range.getSpan());
		*out_transform2 = in_body2.getTransform();
		in_body2.getTransform() = transform2;
	}

	double t = inout_range.getHigh();
	double dt = -inout_range.getSpan() / 2;
	for (uint i = 0; i < in_nIterations; i++) {
		in_body1.update(dt);
		in_body2.update(dt);
		t += dt;
		collision = collisionEvaluator.evaluate(*in_body1.getCollisionMask(), *in_body2.getCollisionMask());
		if (collision.mDid) {
			inout_range.setHigh(t);
			dt = -inout_range.getSpan() / 2;
			inout_collision.mDid = collision.mDid;
			inout_collision.mPoint = collision.mPoint;
		}
		else {
			inout_range.setLow(t);
			dt = inout_range.getSpan() / 2;
			inout_collision.mSeparator = collision.mSeparator;
			inout_collision.mOwner = collision.mOwner;
			if (out_transform1 != nullptr) *out_transform1 = in_body1.getTransform();
			if (out_transform2 != nullptr) *out_transform2 = in_body2.getTransform();
		}
	}

	in_body1.getTransform() = transform1;
	in_body2.getTransform() = transform2;
}

void PhysicsManager::evaluateCollision(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision, Vector2d& out_bounce, Vector2d& out_friction) {
	Vector2d point = in_collision.mPoint;
	Vector2d normal = RigidBody2::getCollisionNormal(in_body1, in_body2, in_collision);

	double restitution = calculateRestitutionCoefficient(in_body1, in_body2);

	Vector2d approachVelocity1 = in_body1.getWorldPointVelocity(point);
	Vector2d approachVelocity2 = in_body2.getWorldPointVelocity(point);
	Vector2d approachVelocity = approachVelocity2 - approachVelocity1;

	double linearMass1 = in_body1.getLinearMass();
	double linearMass2 = in_body2.getLinearMass();

	double angularMass1 = in_body1.getAngularMass();
	double angularMass2 = in_body2.getAngularMass();

	Vector2d radius1 = point - in_body1.getTransform().getWorldPosition();
	Vector2d radius2 = point - in_body2.getTransform().getWorldPosition();

	if (approachVelocity.dot(normal) >= 0) {
		normal = -normal;
	}

	double linearFactor1 = (linearMass1 == 0.0) ? 0.0 : 1.0 / linearMass1;
	double linearFactor2 = (linearMass2 == 0.0) ? 0.0 : 1.0 / linearMass2;

	Vector2d angularFactor1 = (angularMass1 == 0.0) ? Vector2d(0, 0) : radius1.orthogonal() * radius1.magnitude() / angularMass1;
	Vector2d angularFactor2 = (angularMass2 == 0.0) ? Vector2d(0, 0) : radius2.orthogonal() * radius2.magnitude() / angularMass2;
	Vector2d angularFactor = angularFactor1 + angularFactor2;

	double numerator = (1.0 + restitution) * approachVelocity.dot(normal);
	double denominator = linearFactor1 + linearFactor2 + abs(angularFactor.dot(normal));

	Vector2d impulse = (denominator == 0.0) ? Vector2d(0, 0) : normal * (-2.0 + (numerator / denominator));

	Vector2d parallelVelocity = approachVelocity - normal * approachVelocity.dot(normal);
	double frictionCoeff = (parallelVelocity.magnitude() <= slidingEpsilon) ?
		calculateStaticFrictionCoefficient(in_body1, in_body2) :
		calculateKineticFrictionCoefficient(in_body1, in_body2);

	Vector2d friction = parallelVelocity.normalized() * impulse.magnitude() * frictionCoeff;
	Vector2d maxFriction = parallelVelocity / (linearFactor1 + linearFactor2 + abs(angularFactor.dot(parallelVelocity.normalized()))) * 0.99;

	if (friction.dotSelf() > maxFriction.dotSelf()) {
		friction = maxFriction;
	}

	out_bounce = impulse;
	out_friction = friction;
}

typename PhysicsManager::RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status PhysicsManager::evaluateStatus(RigidBody2& in_body1, RigidBody2& in_body2, 
	const Collision2d& in_collision) {

	Vector2d point = in_collision.mPoint;
	Vector2d normal = RigidBody2::getCollisionNormal(in_body1, in_body2, in_collision);
	Vector2d velocity1 = in_body1.getWorldPointVelocity(point);
	Vector2d velocity2 = in_body2.getWorldPointVelocity(point);
	Vector2d velocity = velocity2 - velocity1;
	Vector2d normalVelocity = velocity.projection(normal);
	Vector2d tangentVelocity = velocity - normalVelocity;

	if (normalVelocity.magnitude() <= slidingEpsilon) {
		if (tangentVelocity.magnitude() <= restingEpsilon) {
			return RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status::resting;
		}
		else {
			return RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status::sliding;
		}
	}

	return RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status::bouncing;
}

void PhysicsManager::conformToStatus(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision, RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status in_status) {
	Vector2d point = in_collision.mPoint;
	Vector2d velocity1 = in_body1.getWorldPointVelocity(point);
	Vector2d velocity2 = in_body2.getWorldPointVelocity(point);
}

double PhysicsManager::calculateRestitutionCoefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.4;
}

double PhysicsManager::calculateKineticFrictionCoefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.5;
}

double PhysicsManager::calculateStaticFrictionCoefficient(RigidBody2& in_body1, RigidBody2& in_body2) {
	return 0.6;
}


	