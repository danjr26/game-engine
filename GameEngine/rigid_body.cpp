#include "rigid_body.h"
#include "collision_context.h"
#include "log.h"

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
	return linearVelocity + -in_point.Orthogonal() * angularVelocity.Get_Angle();// *in_point.Magnitude();
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_Local_Point_Velocity(const Vector<double, n>& in_point) {
	return Get_Relative_Point_Velocity(
		in_point - transform.Get_Local_Position()
	);
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_World_Point_Velocity(const Vector<double, n>& in_point) {
	URotation2d rotation = (transform.Get_Parent() == nullptr) ? URotation2d() : transform.Get_World_Rotation().Get_Inverse();
	return Get_Relative_Point_Velocity(
		rotation.Apply_To(in_point - transform.Get_World_Position())
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
	Vector2d linearAcceleration = in_impulse.vector / linearMass;
	URotation2d angularAcceleration = URotation2d(
		in_impulse.vector.Magnitude() * in_impulse.position.Magnitude() * in_impulse.vector.Normalized().Dot(-in_impulse.position.Orthogonal().Normalized()) / angularMass);

	linearVelocity += linearAcceleration;
	angularVelocity += angularAcceleration;
}

template<uint n>
void RigidBody<n>::Apply_Local_Impulse(const LocatedVector<double, n>& in_impulse) {
	Apply_Relative_Impulse({
		in_impulse.position - transform.Get_Local_Position(),
		in_impulse.vector
	});
}

template<uint n>
void RigidBody<n>::Apply_World_Impulse(const LocatedVector<double, n>& in_impulse) {
	URotation2d rotation = (transform.Get_Parent() == nullptr) ? URotation2d() : transform.Get_Parent()->Get_World_Rotation().Get_Inverse();
	Apply_Relative_Impulse({
		rotation.Apply_To(in_impulse.position - transform.Get_World_Position()),
		rotation.Apply_To(in_impulse.vector)
	});
}

Vector2d RigidBody<2>::Impulse_To_Change_Point_Velocity(const Vector2d& in_point, const Vector2d& in_dv) {
	if (Is_Unstoppable()) {
		return Vector2d();
	}

	Vector2d normal = in_dv.Normalized();
	Vector2d radius = in_point - transform.Get_World_Position();
	double linearFactor = (linearMass == 0.0) ? 0.0 : 1.0 / linearMass;
	Vector2d angularFactor = (angularMass == 0.0) ? Vector2d() : radius.Orthogonal() * radius.Magnitude() / angularMass;
	double denominator = linearFactor + angularFactor.Dot(normal);

	return (denominator == 0.0) ? Vector2d() : in_dv / denominator;
}

template<uint n>
void RigidBody<n>::Update(double in_dt) {
	transform.Translate_World(linearVelocity * in_dt);
	transform.Rotate_World(angularVelocity * in_dt);
}

template<uint n>
void RigidBody<n>::Update(double in_dt, const Vector<double, n>& in_normal) {
	Vector<double, n> linearNormalComponent = linearVelocity.Projection(in_normal);
	linearVelocity -= linearNormalComponent;
	Update(in_dt);
	linearVelocity += linearNormalComponent;
}

template<uint n>
Vector<double, n> RigidBody<n>::Get_Collision_Normal(RigidBody<n>& in_body1, RigidBody<n>& in_body2, const Collision<double, n>& in_collision) {
	Vector<double, n> point = in_collision.collisionPoint;
	Vector<double, n> normal1 = in_body1.Get_Collision_Mask()->Get_Closest_Normal(point, CollisionMask2d::PointNormalPolicy::zero);
	Vector<double, n> normal2 = in_body2.Get_Collision_Mask()->Get_Closest_Normal(point, CollisionMask2d::PointNormalPolicy::zero);
	Vector<double, n> normal;

	if (normal1.Is_Zero()) {
		if (normal2.Is_Zero()) {
			normal = in_body1.Get_Collision_Mask()->Get_Closest_Normal(point, CollisionMask2d::PointNormalPolicy::nearest_edge);
		}
		else {
			normal = -normal2;
		}
	}
	else {
		if(normal2.Is_Zero()) {
			normal = normal1;
		}
		else {
			normal = (normal1 - normal2).Normalized();
		}
	}

	return ((in_body1.Get_Transform().Get_World_Position() - in_body2.Get_Transform().Get_World_Position()).Dot(in_collision.separator) >= 0) ? 
		in_collision.separator.Normalized() : -in_collision.separator.Normalized();
}

template class RigidBody<2>;