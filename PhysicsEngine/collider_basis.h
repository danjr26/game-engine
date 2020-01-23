#ifndef COLLIDER_BASIS_H
#define COLLIDER_BASIS_H

#include "collision.h"
#include "transform.h"
#include "misc.h"

template<class T, uint n>
struct ClosestRequest {
	using flags_t = uint;

	enum Fields : flags_t {
		point = 1 << 0,
		normal = 1 << 1
	};

	enum NormalContact {
		on_point,
		on_edge,
		on_face
	};

	Vector<T, n> mPoint;
	Vector<T, n> mNormal;
	NormalContact mContactType;
	flags_t mFlags;

	ClosestRequest() :
	mPoint(),
	mNormal(),
	mContactType(on_point),
	mFlags(0) 
	{}

	ClosestRequest(const Vector<T, n>& i_point, flags_t i_flags = Fields::point) :
	mPoint(i_point),
	mNormal(),
	mContactType(on_point),
	mFlags(i_flags)
	{}

	bool wantsPoint() const {
		return (mFlags & Fields::point);
	}

	bool wantsNormal() const {
		return mFlags & Fields::normal;
	}
};

template<class T, uint n>
class ColliderBasis {
	virtual void applyTransform(const Transform<T, n>& i_transform) = 0;
	virtual void getClosest(ClosestRequest<T, n>& io_request) const = 0;
};

/*
template<class Basis, class T>
class CollisionBasisWrapper 
{};

template<class T>
struct CollisionBasisWrapper<AAHalfSpace<T, 2>, T> {
	using basis_t = AAHalfSpace<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<AxisAlignedLine<T, 2>, T> {
	using basis_t = AxisAlignedLine<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<AABox<T, 2>, T> {
	using basis_t = AABox<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<Sphere<T, 2>, T> {
	using basis_t = Sphere<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<HalfSpace<T, 2>, T> {
	using basis_t = HalfSpace<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<LineSegment<T, 2>, T> {
	using basis_t = LineSegment<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<Line<T, 2>, T> {
	using basis_t = Line<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<MeshSphereTree<T, 2>, T> {
	using basis_t = MeshSphereTree<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<Vector<T, 2>, T> {
	using basis_t = Vector<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<Ray<T, 2>, T> {
	using basis_t = Ray<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<Box<T, 2>, T> {
	using basis_t = Box<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};

template<class T>
struct CollisionBasisWrapper<Triangle<T, 2>, T> {
	using basis_t = Triangle<T, 2>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

	basis_t mBasis;

	void applyTransform(const Transform<T, 2>& i_transform);
	static Vector<T, 2> getClosestPoint(
		const basis_t& i_basis, const Vector<T, 2>& i_point);
	static Vector<T, 2> getClosestNormal(
		const basis_t& i_basis, const Vector<T, 2>& i_point, pnp_t i_policy = pnp_t::towards_point);
};
*/

#endif



