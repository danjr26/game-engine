#include "rigid_body.h"
#include "collision_context.h"
#include "log.h"

template<uint n>
RigidBody<n>::RigidBody() : 
	mCollider(nullptr),
	mLinearVelocity(),
	mAngularVelocity(),
	mLinearMass(0.0),
	mAngularMass(0.0),
	mMultiplier(1.0)
{}

template<uint n>
RigidBody<n>::RigidBody(const Collider<double, n>& i_Collider) :
	mCollider(nullptr),
	mLinearVelocity(),
	mAngularVelocity(),
	mLinearMass(1.0),
	mAngularMass(5000.0) {

	setCollider(i_Collider);
}

template<uint n>
RigidBody<n>::~RigidBody() {
	if (mCollider != nullptr) {
		delete mCollider;
	}
}

template<uint n>
Collider<double, n>* RigidBody<n>::getCollider() {
	return mCollider;
}

template<uint n>
Vector<double, n> RigidBody<n>::getLinearVelocity() const {
	return mLinearVelocity;
}

template<uint n>
URotation<double, n> RigidBody<n>::getAngularVelocity() const {
	return mAngularVelocity;
}

Vector2d RigidBody<2>::getRelativePointVelocity(const Vector2d& i_point) {
	return mLinearVelocity + -i_point.orthogonal() * mAngularVelocity.getAngle();// *i_point.magnitude();
}

template<uint n>
Vector<double, n> RigidBody<n>::getLocalPointVelocity(const Vector<double, n>& i_point) {
	return getRelativePointVelocity(
		i_point - getTransform().getLocalPosition()
	);
}

template<uint n>
Vector<double, n> RigidBody<n>::getWorldPointVelocity(const Vector<double, n>& i_point) {
	URotation2d rotation = (getTransform().getParent() == nullptr) ? URotation2d() : getTransform().getWorldRotation().getInverse();
	return getRelativePointVelocity(
		rotation.applyTo(i_point - getTransform().getWorldPosition())
	);
}

template<uint n>
double RigidBody<n>::getLinearMass() const {
	return mLinearMass;
}

template<uint n>
double RigidBody<n>::getAngularMass() const {
	return mAngularMass;
}

template<uint n>
double RigidBody<n>::getLinearKineticEnergy() const {
	return 0.5 * mLinearMass * mLinearVelocity.dotSelf();
}

template<uint n>
double RigidBody<n>::getAngularKineticEnergy() const {
	return 0.5 * mAngularMass * mAngularVelocity.getAngle() * mAngularVelocity.getAngle();
}

template<uint n>
bool RigidBody<n>::isUnstoppable() const {
	return mCollider->hasFilter(CollisionContext2d::rigid_body_unstoppable);
}

template<uint n>
void RigidBody<n>::setCollider(const Collider<double, n>& i_Collider) {
	if (mCollider != nullptr) {
		delete mCollider;
	}
	mCollider = i_Collider.clone();
	mCollider->getTransform().setParent(&getTransform());
	mCollider->addFilter(CollisionContext2d::rigid_body);
}

template<uint n>
void RigidBody<n>::setLinearVelocity(const Vector<double, n>& i_velocity) {
	mLinearVelocity = i_velocity;
}

template<uint n>
void RigidBody<n>::setAngularVelocity(const URotation<double, n>& i_velocity) {
	mAngularVelocity = i_velocity;
}

template<uint n>
void RigidBody<n>::setLinearMass(double i_mass) {
	mLinearMass = i_mass;
}

template<uint n>
void RigidBody<n>::setAngularMass(double i_mass) {
	mAngularMass = i_mass;
}

template<uint n>
void RigidBody<n>::setUnstoppable(bool i_value) {
	if (i_value) {
		mCollider->addFilter(CollisionContext2d::rigid_body_unstoppable);
		mCollider->removeFilter(CollisionContext2d::rigid_body);
	}
	else {
		mCollider->addFilter(CollisionContext2d::rigid_body);
		mCollider->removeFilter(CollisionContext2d::rigid_body_unstoppable);
	}
}

void RigidBody<2>::applyRelativeImpulse(const LocatedVector<double, 2>& i_impulse) {
	Vector2d linearAcceleration = i_impulse.mVector / mLinearMass;
	URotation2d angularAcceleration = URotation2d(
		i_impulse.mVector.magnitude() * i_impulse.mPosition.magnitude() * i_impulse.mVector.normalized().dot(-i_impulse.mPosition.orthogonal().normalized()) / mAngularMass);

	if(mLinearMass != 0.0) mLinearVelocity += linearAcceleration;
	if(mAngularMass != 0.0) mAngularVelocity += angularAcceleration;
}

template<uint n>
void RigidBody<n>::applyLocalImpulse(const LocatedVector<double, n>& i_impulse) {
	applyRelativeImpulse({
		i_impulse.mPosition - getTransform().getLocalPosition(),
		i_impulse.mVector
	});
}

template<uint n>
void RigidBody<n>::applyWorldImpulse(const LocatedVector<double, n>& i_impulse) {
	URotation2d rotation = (getTransform().getParent() == nullptr) ? URotation2d() : getTransform().getParent()->getWorldRotation().getInverse();
	applyRelativeImpulse({
		rotation.applyTo(i_impulse.mPosition - getTransform().getWorldPosition()),
		rotation.applyTo(i_impulse.mVector)
	});
}

Vector2d RigidBody<2>::impulseToChangePointVelocity(const Vector2d& i_point, const Vector2d& i_dv) {
	if (isUnstoppable()) {
		return Vector2d();
	}

	Vector2d normal = i_dv.normalized();
	Vector2d radius = i_point - getTransform().getWorldPosition();
	double linearFactor = (mLinearMass == 0.0) ? 0.0 : 1.0 / mLinearMass;
	Vector2d angularFactor = (mAngularMass == 0.0) ? Vector2d() : radius.orthogonal() * radius.magnitude() / mAngularMass;
	double denominator = linearFactor + angularFactor.dot(normal);

	return (denominator == 0.0) ? Vector2d() : i_dv / denominator;
}

template<uint n>
void RigidBody<n>::update(double i_dt) {
	getTransform().translateWorld(mLinearVelocity * i_dt * mMultiplier);
	getTransform().rotateWorld(mAngularVelocity * i_dt * mMultiplier);
}

template<uint n>
double RigidBody<n>::getMultiplier() const {
	return mMultiplier;
}

template<uint n>
void RigidBody<n>::setMultiplier(double i_multiplier) {
	mMultiplier = i_multiplier;
}

template<uint n>
Vector<double, n> RigidBody<n>::getCollisionNormal(RigidBody<n>& i_body1, RigidBody<n>& i_body2, const Collision<double, n>& i_collision) {
	//return i_body1.getCollider()->getClosestNormal(i_collision.point, Collider2d::PointNormalPolicy::towards_point);
	if (!i_collision.mSeparator.isZero() && i_collision.mOwner != nullptr) {
		if (i_collision.mOwner == i_body1.getCollider() || i_collision.mOwner == i_body2.getCollider()) {
			return i_collision.mSeparator.normalized() * ((i_collision.mOwner == i_body1.getCollider()) ? 1 : -1);
		}
		else {
			throw InvalidArgumentException();
		}
	}
	else {
		throw InvalidArgumentException();
	}
}

template class RigidBody<2>;