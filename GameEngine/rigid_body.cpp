#include "rigid_body.h"
#include "collision_context.h"
#include "log.h"

template<uint n>
RigidBody<n>::RigidBody() : 
	mCollisionMask(nullptr),
	mLinearVelocity(),
	mAngularVelocity(),
	mLinearMass(0.0),
	mAngularMass(0.0)
{}

template<uint n>
RigidBody<n>::RigidBody(const CollisionMask<double, n>& in_collisionMask) :
	mCollisionMask(nullptr),
	mLinearVelocity(),
	mAngularVelocity(),
	mLinearMass(1.0),
	mAngularMass(5000.0) {

	setCollisionMask(in_collisionMask);
}

template<uint n>
RigidBody<n>::~RigidBody() {
	if (mCollisionMask != nullptr) {
		delete mCollisionMask;
	}
}

template<uint n>
CollisionMask<double, n>* RigidBody<n>::getCollisionMask() {
	return mCollisionMask;
}

template<uint n>
Vector<double, n> RigidBody<n>::getLinearVelocity() const {
	return mLinearVelocity;
}

template<uint n>
URotation<double, n> RigidBody<n>::getAngularVelocity() const {
	return mAngularVelocity;
}

Vector2d RigidBody<2>::getRelativePointVelocity(const Vector2d& in_point) {
	return mLinearVelocity + -in_point.orthogonal() * mAngularVelocity.getAngle();// *in_point.magnitude();
}

template<uint n>
Vector<double, n> RigidBody<n>::getLocalPointVelocity(const Vector<double, n>& in_point) {
	return getRelativePointVelocity(
		in_point - mTransform.getLocalPosition()
	);
}

template<uint n>
Vector<double, n> RigidBody<n>::getWorldPointVelocity(const Vector<double, n>& in_point) {
	URotation2d rotation = (mTransform.getParent() == nullptr) ? URotation2d() : mTransform.getWorldRotation().getInverse();
	return getRelativePointVelocity(
		rotation.applyTo(in_point - mTransform.getWorldPosition())
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
	return mCollisionMask->hasFilter(CollisionContext2d::rigid_body_unstoppable);
}

template<uint n>
void RigidBody<n>::setCollisionMask(const CollisionMask<double, n>& in_collisionMask) {
	if (mCollisionMask != nullptr) {
		delete mCollisionMask;
	}
	mCollisionMask = in_collisionMask.clone();
	mCollisionMask->getTransform().setParent(&getTransform());
	mCollisionMask->addFilter(CollisionContext2d::rigid_body);
}

template<uint n>
void RigidBody<n>::setLinearVelocity(const Vector<double, n>& in_velocity) {
	mLinearVelocity = in_velocity;
}

template<uint n>
void RigidBody<n>::setAngularVelocity(const URotation<double, n>& in_velocity) {
	mAngularVelocity = in_velocity;
}

template<uint n>
void RigidBody<n>::setLinearMass(double in_mass) {
	mLinearMass = in_mass;
}

template<uint n>
void RigidBody<n>::setAngularMass(double in_mass) {
	mAngularMass = in_mass;
}

template<uint n>
void RigidBody<n>::setUnstoppable(bool in_value) {
	if (in_value) {
		mCollisionMask->addFilter(CollisionContext2d::rigid_body_unstoppable);
		mCollisionMask->removeFilter(CollisionContext2d::rigid_body);
	}
	else {
		mCollisionMask->addFilter(CollisionContext2d::rigid_body);
		mCollisionMask->removeFilter(CollisionContext2d::rigid_body_unstoppable);
	}
}

void RigidBody<2>::applyRelativeImpulse(const LocatedVector<double, 2>& in_impulse) {
	Vector2d linearAcceleration = in_impulse.mVector / mLinearMass;
	URotation2d angularAcceleration = URotation2d(
		in_impulse.mVector.magnitude() * in_impulse.mPosition.magnitude() * in_impulse.mVector.normalized().dot(-in_impulse.mPosition.orthogonal().normalized()) / mAngularMass);

	if(mLinearMass != 0.0) mLinearVelocity += linearAcceleration;
	if(mAngularMass != 0.0) mAngularVelocity += angularAcceleration;
}

template<uint n>
void RigidBody<n>::applyLocalImpulse(const LocatedVector<double, n>& in_impulse) {
	applyRelativeImpulse({
		in_impulse.mPosition - mTransform.getLocalPosition(),
		in_impulse.mVector
	});
}

template<uint n>
void RigidBody<n>::applyWorldImpulse(const LocatedVector<double, n>& in_impulse) {
	URotation2d rotation = (mTransform.getParent() == nullptr) ? URotation2d() : mTransform.getParent()->getWorldRotation().getInverse();
	applyRelativeImpulse({
		rotation.applyTo(in_impulse.mPosition - mTransform.getWorldPosition()),
		rotation.applyTo(in_impulse.mVector)
	});
}

Vector2d RigidBody<2>::impulseToChangePointVelocity(const Vector2d& in_point, const Vector2d& in_dv) {
	if (isUnstoppable()) {
		return Vector2d();
	}

	Vector2d normal = in_dv.normalized();
	Vector2d radius = in_point - mTransform.getWorldPosition();
	double linearFactor = (mLinearMass == 0.0) ? 0.0 : 1.0 / mLinearMass;
	Vector2d angularFactor = (mAngularMass == 0.0) ? Vector2d() : radius.orthogonal() * radius.magnitude() / mAngularMass;
	double denominator = linearFactor + angularFactor.dot(normal);

	return (denominator == 0.0) ? Vector2d() : in_dv / denominator;
}

template<uint n>
void RigidBody<n>::update(double in_dt) {
	mTransform.translateWorld(mLinearVelocity * in_dt);
	mTransform.rotateWorld(mAngularVelocity * in_dt);
}

template<uint n>
void RigidBody<n>::update(double in_dt, const Vector<double, n>& in_normal) {
	Vector<double, n> linearNormalComponent = mLinearVelocity.projection(in_normal);
	mLinearVelocity -= linearNormalComponent;
	update(in_dt);
	mLinearVelocity += linearNormalComponent;
}

template<uint n>
Vector<double, n> RigidBody<n>::getCollisionNormal(RigidBody<n>& in_body1, RigidBody<n>& in_body2, const Collision<double, n>& in_collision) {
	//return in_body1.getCollisionMask()->getClosestNormal(in_collision.point, CollisionMask2d::PointNormalPolicy::towards_point);
	if (!in_collision.mSeparator.isZero() && in_collision.mOwner != nullptr) {
		if (in_collision.mOwner == in_body1.getCollisionMask() || in_collision.mOwner == in_body2.getCollisionMask()) {
			return in_collision.mSeparator.normalized() * ((in_collision.mOwner == in_body1.getCollisionMask()) ? 1 : -1);
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