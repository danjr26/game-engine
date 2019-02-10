#include "rigid_body.h"
#include "collision_context.h"
#include "log.h"

template<uint n>
RigidBody<n>::RigidBody(const CollisionMask<double, n>& in_collisionMask) :
	mCollisionMask(in_collisionMask.Clone()),
	mLinearVelocity(),
	mAngularVelocity(),
	mLinearMass(1.0),
	mAngularMass(5000.0) {

	mCollisionMask->Get_Transform().Set_Parent(&Get_Transform());
	mCollisionMask->Add_Filter(CollisionContext2d::rigid_body);
}

template<uint n>
RigidBody<n>::~RigidBody() {
	delete mCollisionMask;
}

template<uint n>
CollisionMask<double, n>* RigidBody<n>::Get_Collision_Mask() {
	return mCollisionMask;
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_Linear_Velocity() const {
	return mLinearVelocity;
}

template<uint n>
URotation<double, n> RigidBody<n>::Get_Angular_Velocity() const {
	return mAngularVelocity;
}

Vector2d RigidBody<2>::Get_Relative_Point_Velocity(const Vector2d& in_point) {
	return mLinearVelocity + -in_point.Orthogonal() * mAngularVelocity.Get_Angle();// *in_point.Magnitude();
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_Local_Point_Velocity(const Vector<double, n>& in_point) {
	return Get_Relative_Point_Velocity(
		in_point - mTransform.Get_Local_Position()
	);
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_World_Point_Velocity(const Vector<double, n>& in_point) {
	URotation2d rotation = (mTransform.Get_Parent() == nullptr) ? URotation2d() : mTransform.Get_World_Rotation().Get_Inverse();
	return Get_Relative_Point_Velocity(
		rotation.Apply_To(in_point - mTransform.Get_World_Position())
	);
}

template<uint n>
double RigidBody<n>::Get_Linear_Mass() const {
	return mLinearMass;
}

template<uint n>
double RigidBody<n>::Get_Angular_Mass() const {
	return mAngularMass;
}

template<uint n>
double RigidBody<n>::Get_Linear_Kinetic_Energy() const {
	return 0.5 * mLinearMass * mLinearVelocity.Dot_Self();
}

template<uint n>
double RigidBody<n>::Get_Angular_Kinetic_Energy() const {
	return 0.5 * mAngularMass * mAngularVelocity.Get_Angle() * mAngularVelocity.Get_Angle();
}

template<uint n>
bool RigidBody<n>::Is_Unstoppable() const {
	return mCollisionMask->Has_Filter(CollisionContext2d::rigid_body_unstoppable);
}

template<uint n>
void RigidBody<n>::Set_Linear_Velocity(const Vector<double, n>& in_velocity) {
	mLinearVelocity = in_velocity;
}

template<uint n>
void RigidBody<n>::Set_Angular_Velocity(const URotation<double, n>& in_velocity) {
	mAngularVelocity = in_velocity;
}

template<uint n>
void RigidBody<n>::Set_Linear_Mass(double in_mass) {
	mLinearMass = in_mass;
}

template<uint n>
void RigidBody<n>::Set_Angular_Mass(double in_mass) {
	mAngularMass = in_mass;
}

template<uint n>
void RigidBody<n>::Set_Unstoppable(bool in_value) {
	if (in_value) {
		mCollisionMask->Add_Filter(CollisionContext2d::rigid_body_unstoppable);
		mCollisionMask->Remove_Filter(CollisionContext2d::rigid_body);
	}
	else {
		mCollisionMask->Add_Filter(CollisionContext2d::rigid_body);
		mCollisionMask->Remove_Filter(CollisionContext2d::rigid_body_unstoppable);
	}
}

void RigidBody<2>::Apply_Relative_Impulse(const LocatedVector<double, 2>& in_impulse) {
	Vector2d linearAcceleration = in_impulse.mVector / mLinearMass;
	URotation2d angularAcceleration = URotation2d(
		in_impulse.mVector.Magnitude() * in_impulse.mPosition.Magnitude() * in_impulse.mVector.Normalized().Dot(-in_impulse.mPosition.Orthogonal().Normalized()) / mAngularMass);

	mLinearVelocity += linearAcceleration;
	mAngularVelocity += angularAcceleration;
}

template<uint n>
void RigidBody<n>::Apply_Local_Impulse(const LocatedVector<double, n>& in_impulse) {
	Apply_Relative_Impulse({
		in_impulse.mPosition - mTransform.Get_Local_Position(),
		in_impulse.mVector
	});
}

template<uint n>
void RigidBody<n>::Apply_World_Impulse(const LocatedVector<double, n>& in_impulse) {
	URotation2d rotation = (mTransform.Get_Parent() == nullptr) ? URotation2d() : mTransform.Get_Parent()->Get_World_Rotation().Get_Inverse();
	Apply_Relative_Impulse({
		rotation.Apply_To(in_impulse.mPosition - mTransform.Get_World_Position()),
		rotation.Apply_To(in_impulse.mVector)
	});
}

Vector2d RigidBody<2>::Impulse_To_Change_Point_Velocity(const Vector2d& in_point, const Vector2d& in_dv) {
	if (Is_Unstoppable()) {
		return Vector2d();
	}

	Vector2d normal = in_dv.Normalized();
	Vector2d radius = in_point - mTransform.Get_World_Position();
	double linearFactor = (mLinearMass == 0.0) ? 0.0 : 1.0 / mLinearMass;
	Vector2d angularFactor = (mAngularMass == 0.0) ? Vector2d() : radius.Orthogonal() * radius.Magnitude() / mAngularMass;
	double denominator = linearFactor + angularFactor.Dot(normal);

	return (denominator == 0.0) ? Vector2d() : in_dv / denominator;
}

template<uint n>
void RigidBody<n>::Update(double in_dt) {
	mTransform.Translate_World(mLinearVelocity * in_dt);
	mTransform.Rotate_World(mAngularVelocity * in_dt);
}

template<uint n>
void RigidBody<n>::Update(double in_dt, const Vector<double, n>& in_normal) {
	Vector<double, n> linearNormalComponent = mLinearVelocity.Projection(in_normal);
	mLinearVelocity -= linearNormalComponent;
	Update(in_dt);
	mLinearVelocity += linearNormalComponent;
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_Collision_Normal(RigidBody<n>& in_body1, RigidBody<n>& in_body2, const Collision<double, n>& in_collision) {
	//return in_body1.Get_Collision_Mask()->Get_Closest_Normal(in_collision.point, CollisionMask2d::PointNormalPolicy::towards_point);
	if (!in_collision.mSeparator.Is_Zero() && in_collision.mOwner != nullptr) {
		if (in_collision.mOwner == in_body1.Get_Collision_Mask() || in_collision.mOwner == in_body2.Get_Collision_Mask()) {
			return in_collision.mSeparator.Normalized() * ((in_collision.mOwner == in_body1.Get_Collision_Mask()) ? 1 : -1);
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