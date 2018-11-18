#include "rigid_body.h"
#include "collision_context.h"

template<uint n>
RigidBody<n>::RigidBody(const CollisionMask<double, n>& in_collisionMask) :
	collisionMask(in_collisionMask.Clone()),
	linearVelocity(),
	angularVelocity(),
	linearMass(1.0),
	angularMass(5000.0) {

	collisionMask->Get_Transform().Set_Parent(&Get_Transform());
	collisionMask->Add_Filter(CollisionContext2d::rigid_body);
}

template<uint n>
RigidBody<n>::~RigidBody() {
	delete collisionMask;
}

template<uint n>
CollisionMask<double, n>* RigidBody<n>::Get_Collision_Mask() {
	return collisionMask;
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_Linear_Velocity() const {
	return linearVelocity;
}

template<uint n>
URotation<double, n> RigidBody<n>::Get_Angular_Velocity() const {
	return angularVelocity;
}

Vector2d RigidBody<2>::Get_Relative_Point_Velocity(const Vector2d& in_point) {
	return linearVelocity + in_point.Orthogonal() * angularVelocity.Get_Angle() * in_point.Magnitude();
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_Local_Point_Velocity(const Vector<double, n>& in_point) {
	return Get_Relative_Point_Velocity(
		transform.Get_Local_Rotation().Get_Inverse().Apply_To(in_point - transform.Get_Local_Position())
	);
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_World_Point_Velocity(const Vector<double, n>& in_point) {
	return Get_Relative_Point_Velocity(
		transform.Get_World_Rotation().Get_Inverse().Apply_To(in_point - transform.Get_World_Position())
	);
}

template<uint n>
double RigidBody<n>::Get_Linear_Mass() const {
	return linearMass;
}

template<uint n>
double RigidBody<n>::Get_Angular_Mass() const {
	return angularMass;
}

template<uint n>
double RigidBody<n>::Get_Linear_Kinetic_Energy() const {
	return 0.5 * linearMass * linearVelocity.Dot_Self();
}

template<uint n>
double RigidBody<n>::Get_Angular_Kinetic_Energy() const {
	return 0.5 * angularMass * angularVelocity.Get_Angle() * angularVelocity.Get_Angle();
}

template<uint n>
bool RigidBody<n>::Is_Unstoppable() const {
	return collisionMask->Has_Filter(CollisionContext2d::rigid_body_unstoppable);
}

template<uint n>
void RigidBody<n>::Set_Linear_Velocity(const Vector<double, n>& in_velocity) {
	linearVelocity = in_velocity;
}

template<uint n>
void RigidBody<n>::Set_Angular_Velocity(const URotation<double, n>& in_velocity) {
	angularVelocity = in_velocity;
}

template<uint n>
void RigidBody<n>::Set_Linear_Mass(double in_mass) {
	linearMass = in_mass;
}

template<uint n>
void RigidBody<n>::Set_Angular_Mass(double in_mass) {
	angularMass = in_mass;
}

template<uint n>
void RigidBody<n>::Set_Unstoppable(bool in_value) {
	if (in_value) {
		collisionMask->Add_Filter(CollisionContext2d::rigid_body_unstoppable);
		collisionMask->Remove_Filter(CollisionContext2d::rigid_body);
	}
	else {
		collisionMask->Add_Filter(CollisionContext2d::rigid_body);
		collisionMask->Remove_Filter(CollisionContext2d::rigid_body_unstoppable);
	}
}

void RigidBody<2>::Apply_Relative_Impulse(const LocatedVector<double, 2>& in_impulse) {
	linearVelocity += in_impulse.vector / linearMass;
	angularVelocity += URotation2d(in_impulse.vector.Magnitude() * sin(in_impulse.vector.Theta(in_impulse.position)) / angularMass);
}

template<uint n>
void RigidBody<n>::Apply_Local_Impulse(const LocatedVector<double, n>& in_impulse) {
	Apply_Relative_Impulse({
		transform.Get_Local_Rotation().Get_Inverse().Apply_To(in_impulse.position - transform.Get_Local_Position()),
		transform.Get_Local_Rotation().Get_Inverse().Apply_To(in_impulse.vector)
	});
}

template<uint n>
void RigidBody<n>::Apply_World_Impulse(const LocatedVector<double, n>& in_impulse) {
	Apply_Relative_Impulse({
		transform.Get_World_Rotation().Get_Inverse().Apply_To(in_impulse.position - transform.Get_World_Position()),
		transform.Get_World_Rotation().Get_Inverse().Apply_To(in_impulse.vector)
	});
}

template<uint n>
void RigidBody<n>::Update(double in_dt) {
	transform.Translate_World(linearVelocity * in_dt);
	transform.Rotate_World(angularVelocity * in_dt);
}

template class RigidBody<2>;