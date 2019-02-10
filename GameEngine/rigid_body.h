#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "collision_mask.h"
#include "vector.h"
#include "unclamped_rotation.h"
#include "located_vector.h"
#include "range.h"

template<uint n>
class RigidBody : public TransformableObject<double, n> {
private:
	CollisionMask<double, n>* mCollisionMask;
	Vector<double, n> mLinearVelocity;
	URotation<double, n> mAngularVelocity;
	double mLinearMass;
	double mAngularMass;

public:
	RigidBody(const CollisionMask<double, n>& in_collisionMask);
	~RigidBody();

	CollisionMask<double, n>* getCollisionMask();
	Vector<double, n> getLinearVelocity() const;
	URotation<double, n> getAngularVelocity() const;
	Vector<double, n> getRelativePointVelocity(const Vector<double, n>& in_point);
	Vector<double, n> getLocalPointVelocity(const Vector<double, n>& in_point);
	Vector<double, n> getWorldPointVelocity(const Vector<double, n>& in_point);
	double getLinearMass() const;
	double getAngularMass() const;
	double getLinearKineticEnergy() const;
	double getAngularKineticEnergy() const;
	bool isUnstoppable() const;

	void setLinearVelocity(const Vector<double, n>& in_velocity);
	void setAngularVelocity(const URotation<double, n>& in_velocity);
	void setLinearMass(double in_mass);
	void setAngularMass(double in_mass);
	void setUnstoppable(bool in_value);

	void applyRelativeImpulse(const LocatedVector<double, n>& in_impulse);
	void applyLocalImpulse(const LocatedVector<double, n>& in_impulse);
	void applyWorldImpulse(const LocatedVector<double, n>& in_impulse);

	Vector<double, n> impulseToChangePointVelocity(const Vector<double, n>& in_point, const Vector<double, n>& in_dv);

	void update(double in_dt);
	void update(double in_dt, const Vector<double, n>& in_normal);

	static Vector<double, n> getCollisionNormal(RigidBody<n>& in_body1, RigidBody<n>& in_body2, const Collision<double, n>& in_collision);
};

using RigidBody2 = RigidBody<2>;
using RigidBody3 = RigidBody<3>;

#endif


