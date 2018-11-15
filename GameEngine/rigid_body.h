#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "collision_mask.h"
#include "vector.h"
#include "unclamped_rotation.h"

template<uint n>
class RigidBody : public TransformableObject<double, n> {
private:
	CollisionMask<double, n>* collisionMask;
	Vector<double, n> linearVelocity;
	URotation<double, n> angularVelocity;
	double linearMass;
	double angularMass;

public:
	RigidBody(const CollisionMask<double, n>& in_collisionMask);
	~RigidBody();

	CollisionMask<double, n>* Get_Collision_Mask();
	Vector<double, n> Get_Linear_Velocity() const;
	URotation<double, n> Get_Angular_Velocity() const;
	Vector<double, n> Get_Relative_Point_Velocity(const Vector<double, n>& in_point);
	Vector<double, n> Get_Local_Point_Velocity(const Vector<double, n>& in_point);
	Vector<double, n> Get_World_Point_Velocity(const Vector<double, n>& in_point);
	double Get_Linear_Mass() const;
	double Get_Angular_Mass() const;
	double Get_Linear_Kinetic_Energy() const;
	double Get_Angular_Kinetic_Energy() const;
	bool Is_Unstoppable() const;

	void Set_Linear_Velocity(const Vector<double, n>& in_velocity);
	void Set_Angular_Velocity(const URotation<double, n>& in_velocity);
	void Set_Linear_Mass(double in_mass);
	void Set_Angular_Mass(double in_mass);
	void Set_Unstoppable(bool in_value);

	void Apply_Relative_Impulse(const Vector<double, n>& in_impulse, const Vector<double, n>& in_position);
	void Apply_Local_Impulse(const Vector<double, n>& in_impulse, const Vector<double, n>& in_position);
	void Apply_World_Impulse(const Vector<double, n>& in_impulse, const Vector<double, n>& in_position);

	void Update(double in_dt);
};

using RigidBody2 = RigidBody<2>;
using RigidBody3 = RigidBody<3>;

#endif


