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

	void update(double i_dt);

	void add(RigidBody2* i_rigidBody);
	void add(ForceField2* i_forceField);
	void remove(RigidBody2* i_rigidBody);
	void remove(ForceField2* i_forceField);
	bool has(RigidBody2* i_rigidBody);
	bool has(ForceField2* i_forceField);

private:
	void prepareDataContainers(double i_dt);
	void excludePreembedded(double i_dt);
	void stepAll(double i_dt);

	void applyRigidBodiesToRigidBodies(double i_dt);
	void applyForceFieldsToRigidBodies(double i_dt);

	void calculateForceFieldEffects(double i_dt);
	void calculateRigidBodyEffects(double i_dt);
	void applyEffectsToRigidBodies(double i_dt);
	void narrowCollisionInterval(RigidBody2& i_body1, RigidBody2& i_body2, Collision2d& io_collision, Ranged& io_range, uint i_nIterations, 
		Transform2d* o_transform1 = nullptr, Transform2d* o_transform2 = nullptr);
	void evaluateCollision(RigidBody2& i_body1, RigidBody2& i_body2, const Collision2d& i_collision, Vector2d& o_bounce, Vector2d& o_friction);

	RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status evaluateStatus(RigidBody2& i_body1, RigidBody2& i_body2, const Collision2d& i_collision);
	void conformToStatus(RigidBody2& i_body1, RigidBody2& i_body2, const Collision2d& i_collision, RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status i_status);
	static double calculateRestitutionCoefficient(RigidBody2& i_body1, RigidBody2& i_body2);
	static double calculateKineticFrictionCoefficient(RigidBody2& i_body1, RigidBody2& i_body2);
	static double calculateStaticFrictionCoefficient(RigidBody2& i_body1, RigidBody2& i_body2);
};

#endif