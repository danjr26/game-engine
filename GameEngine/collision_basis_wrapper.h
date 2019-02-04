#ifndef COLLISION_BASIS_WRAPPER_H
#define COLLISION_BASIS_WRAPPER_H

#include "collision_mask.h"
#include "misc.h"

template<class Basis, class T>
class CollisionBasisWrapper 
{};

template<class T>
struct CollisionBasisWrapper<AxisAlignedHalfSpace<T, 2>, T> {
	using basis_t = AxisAlignedHalfSpace<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<AxisAlignedLine<T, 2>, T> {
	using basis_t = AxisAlignedLine<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<AxisAlignedBox<T, 2>, T> {
	using basis_t = AxisAlignedBox<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<Sphere<T, 2>, T> {
	using basis_t = Sphere<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<HalfSpace<T, 2>, T> {
	using basis_t = HalfSpace<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<LineSegment<T, 2>, T> {
	using basis_t = LineSegment<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<Line<T, 2>, T> {
	using basis_t = Line<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<MeshSphereTree<T, 2>, T> {
	using basis_t = MeshSphereTree<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<Vector<T, 2>, T> {
	using basis_t = Vector<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<Ray<T, 2>, T> {
	using basis_t = Ray<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<Box<T, 2>, T> {
	using basis_t = Box<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

template<class T>
struct CollisionBasisWrapper<Triangle<T, 2>, T> {
	using basis_t = Triangle<T, 2>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

	basis_t basis;

	void Apply_Transform(const Transform<T, 2>& in_transform);
	static Vector<T, 2> Get_Closest_Point(
		const basis_t& in_basis, const Vector<T, 2>& in_point);
	static Vector<T, 2> Get_Closest_Normal(
		const basis_t& in_basis, const Vector<T, 2>& in_point, pnp_t in_policy = towards_point);
};

#endif



