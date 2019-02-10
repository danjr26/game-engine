#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <vector>
#include <unordered_map>
#include "rigid_body.h"
#include "collision_context.h"
#include "range.h"
#include "force_field.h"

class PhysicsManager {
private:
	template<uint n>
	struct RigidBodyUpdateInfo {
		struct RigidBodyEffectInfo {
			Transform2d transform;
			RigidBody<n>* body;
			LocatedVector<double, n> impulse;
			Vector<double, n> normal;
			Ranged interval;
			double finishTime;
			enum Status {
				bouncing,
				sliding,
				resting
			} status;
		};

		struct ForceFieldEffectInfo {
			Transform2d transform;
			ForceField<n>* field;
			LocatedVector<double, n> impulse;
		};

		RigidBodyEffectInfo rigidBodyInfo;
		std::vector<ForceFieldEffectInfo> forceFieldInfo;
	};
	using RigidBodyUpdateInfo2 = RigidBodyUpdateInfo<2>;
	using RigidBodyUpdateInfo3 = RigidBodyUpdateInfo<3>;

	CollisionContext2d collisionContext;
	std::vector<RigidBody2*> rigidBody2s;
	std::vector<ForceField2*> forceField2s;

	std::unordered_map<RigidBody2*, RigidBodyUpdateInfo2> rigidBody2UpdateInfo;

	uint collisionNarrowSteps;
	uint maxStepRepeat;
	
	// minimum distance to be considered sliding
	double slidingEpsilon; 

	// maximum velocity to be considered resting
	double restingEpsilon;

public:
	PhysicsManager();

	void update(double in_dt);

	void add(RigidBody2* in_rigidBody);
	void add(ForceField2* in_forceField);
	void remove(RigidBody2* in_rigidBody);
	void remove(ForceField2* in_forceField);
	bool has(RigidBody2* in_rigidBody);
	bool has(ForceField2* in_forceField);

private:
	void prepareDataContainers(double in_dt);
	void excludePreembedded(double in_dt);
	void stepAll(double in_dt);

	void applyRigidBodiesToRigidBodies(double in_dt);
	void applyForceFieldsToRigidBodies(double in_dt);

	void calculateForceFieldEffects(double in_dt);
	void calculateRigidBodyEffects(double in_dt);
	void applyEffectsToRigidBodies(double in_dt);
	void narrowCollisionInterval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& inout_collision, Ranged& inout_range, uint in_nIterations, 
		Transform2d* out_transform1 = nullptr, Transform2d* out_transform2 = nullptr);
	void evaluateCollision(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision, Vector2d& out_bounce, Vector2d& out_friction);

	RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status evaluateStatus(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision);
	void conformToStatus(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision, RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status in_status);
	static double calculateRestitutionCoefficient(RigidBody2& in_body1, RigidBody2& in_body2);
	static double calculateKineticFrictionCoefficient(RigidBody2& in_body1, RigidBody2& in_body2);
	static double calculateStaticFrictionCoefficient(RigidBody2& in_body1, RigidBody2& in_body2);
};

#endif