#include "rigid_body.h"

template<uint n>
inline RigidBody<n>::RigidBody(const CollisionMask<double, n>& in_collisionMask) :
	collisionMask(in_collisionMask.Clone()),
	linearVelocity(),
	angularVelocity(),
	linearMass(1.0),
	angularMass(0.4)
{
	collisionMask->Get_Transform().Set_Parent(&transform);
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

void RigidBody<2>::Apply_Relative_Impulse(const Vector2d& in_impulse, const Vector2d& in_position) {
	linearVelocity += in_impulse / linearMass;
	angularVelocity += URotation2d(
		in_impulse.Magnitude() * sin(in_impulse.Theta(in_position)) /
		(2.0 * PI * angularMass)
	);
}

template<uint n>
void RigidBody<n>::Apply_Local_Impulse(const Vector<double, n>& in_impulse, const Vector<double, n>& in_position) {
	Apply_Relative_Impulse(
		transform.Get_Local_Rotation().Get_Inverse().Apply_To(in_impulse), 
		transform.Get_Local_Rotation().Get_Inverse().Apply_To(in_position - transform.Get_Local_Position())
	);
}

template<uint n>
void RigidBody<n>::Apply_World_Impulse(const Vector<double, n>& in_impulse, const Vector<double, n>& in_position) {
	Apply_Relative_Impulse(
		transform.Get_World_Rotation().Get_Inverse().Apply_To(in_impulse),
		transform.Get_World_Rotation().Get_Inverse().Apply_To(in_position - transform.Get_World_Position())
	);
}

template<uint n>
void RigidBody<n>::Update(double in_dt) {
	transform.Translate_Local(linearVelocity * in_dt);
	transform.Rotate_Local(angularVelocity * in_dt);
}

template class RigidBody<2>;