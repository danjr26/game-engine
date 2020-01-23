#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "collider.h"
#include "vector.h"
#include "unclamped_rotation.h"
#include "located_vector.h"
#include "range.h"

template<uint n>
class RigidBody : public TransformableObject<double, n> {
private:
	Collider<double, n>* mCollider;
	Vector<double, n> mLinearVelocity;
	URotation<double, n> mAngularVelocity;
	double mLinearMass;
	double mAngularMass;
	double mMultiplier;

public:
	RigidBody();
	RigidBody(const Collider<double, n>& i_Collider);
	~RigidBody();

	Collider<double, n>* getCollider();
	Vector<double, n> getLinearVelocity() const;
	URotation<double, n> getAngularVelocity() const;
	Vector<double, n> getRelativePointVelocity(const Vector<double, n>& i_point);
	Vector<double, n> getLocalPointVelocity(const Vector<double, n>& i_point);
	Vector<double, n> getWorldPointVelocity(const Vector<double, n>& i_point);
	double getLinearMass() const;
	double getAngularMass() const;
	double getLinearKineticEnergy() const;
	double getAngularKineticEnergy() const;
	bool isUnstoppable() const;

	void setCollider(const Collider<double, n>& i_Collider);
	void setLinearVelocity(const Vector<double, n>& i_velocity);
	void setAngularVelocity(const URotation<double, n>& i_velocity);
	void setLinearMass(double i_mass);
	void setAngularMass(double i_mass);
	void setUnstoppable(bool i_value);

	void applyRelativeImpulse(const LocatedVector<double, n>& i_impulse);
	void applyLocalImpulse(const LocatedVector<double, n>& i_impulse);
	void applyWorldImpulse(const LocatedVector<double, n>& i_impulse);

	Vector<double, n> impulseToChangePointVelocity(const Vector<double, n>& i_point, const Vector<double, n>& i_dv);

	void update(double i_dt);

	double getMultiplier() const;
	void setMultiplier(double i_multiplier);

	static Vector<double, n> getCollisionNormal(RigidBody<n>& i_body1, RigidBody<n>& i_body2, const Collision<double, n>& i_collision);
};

using RigidBody2 = RigidBody<2>;
using RigidBody3 = RigidBody<3>;

#endif


