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
			RigidBody<n>* body;
			LocatedVector<double, n> impulse;
			double precollisionTime;
			enum Status {
				bouncing,
				sliding,
				resting
			} status;
		};

		struct ForceFieldEffectInfo {
			ForceField<n>* field;
			LocatedVector<double, n> impulse;
		};

		std::vector<RigidBodyEffectInfo> rigidBodyInfo;
		std::vector<ForceFieldEffectInfo> forceFieldInfo;
	};
	using RigidBodyUpdateInfo2 = RigidBodyUpdateInfo<2>;
	using RigidBodyUpdateInfo3 = RigidBodyUpdateInfo<3>;

	CollisionContext2d collisionContext;
	std::vector<RigidBody2*> rigidBody2s;
	std::vector<ForceField2*> forceField2s;

	std::unordered_map<RigidBody2*, RigidBodyUpdateInfo2> rigidBody2UpdateInfo;

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
	const int collisionNarrowSteps = 5;

	void Prepare_Data_Containers();
	void Exclude_Preembedded();
	void Step_All(double in_dt);
	void Calculate_Force_Field_Effects(double in_dt);
	void Calculate_Rigid_Body_Effects(double in_dt);
	void Apply_Effects_To_Rigid_Bodies(double in_dt);
	void Narrow_Collision_Interval(RigidBody2& in_body1, RigidBody2& in_body2, Collision2d& inout_collision, Ranged& inout_range, uint in_nIterations);
	void Narrow_Collision_Interval(RigidBody2& in_body, RigidBody2*& out_otherBody, Collision2d& out_collision, Ranged& inout_range, uint in_nIterations);
	Vector2d Evaluate_Collision(RigidBody2& in_body1, RigidBody2& in_body2, const Collision2d& in_collision);
	static double Calculate_Restitution_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
	static double Calculate_Kinetic_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
	static double Calculate_Static_Friction_Coefficient(RigidBody2& in_body1, RigidBody2& in_body2);
};

#endif