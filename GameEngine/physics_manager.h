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
	double slidingEpsilon;
	double restingEpsilon;

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
	void Prepare_Data_Containers(double in_dt);
	void Exclude_Preembedded(double in_dt);
	void Step_All(double in_dt);

	void Apply_Rigid_Bodies_To_Rigid_Bodies(double in_dt);
	void Apply_Force_Fields_To_Rigid_Bodies(double in_dt);

	void Calculate_Force_Field_Effects(double in_dt);
	void Calculate_Rigid_Body_Effects(double in_dt);
	void Apply_Effects_To_Rigid_Bodies(double in_dt);
	void Narrow_Collision_Interval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& inout_collision, Ranged& inout_range, uint in_nIterations);
	void Evaluate_Collision(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision, Vector2d& out_bounce, Vector2d& out_friction);

	RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status Evaluate_Status(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision);
	void Conform_To_Status(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision, RigidBodyUpdateInfo2::RigidBodyEffectInfo::Status in_status);
	static double Calculate_Restitution_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
	static double Calculate_Kinetic_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
	static double Calculate_Static_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
};

#endif