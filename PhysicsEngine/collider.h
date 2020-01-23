#ifndef COLLIDER_H
#define COLLIDER_H

#include "collider_basis.h"
#include "transformable_object.h"
#include "collision_queue.h"
#include "misc.h"

#include "axis_aligned_half_space.h"
#include "axis_aligned_box.h"
#include "sphere.h"
#include "half_space.h"
#include "mesh_sphere_tree.h"
#include "line.h"
#include "line_segment.h"
#include "ray.h"
#include "box.h"
#include "triangle.h"
#include "point_collider_basis.h"

template<class T, uint n, class Basis>
class _Collider;

template<class T, uint n>
using AAHalfSpaceCollider = _Collider<T, n, AAHalfSpace<T, n>>;
template<class T, uint n>
using AABoxCollider = _Collider<T, n, AABox<T, n>>;
template<class T, uint n>
using SphereCollider = _Collider<T, n, Sphere<T, n>>;
template<class T, uint n>
using HalfSpaceCollider = _Collider<T, n, HalfSpace<T, n>>;
template<class T, uint n>
using LineCollider = _Collider<T, n, Line<T, n>>;
template<class T, uint n>
using LineSegmentCollider = _Collider<T, n, LineSegment<T, n>>;
template<class T, uint n>
using MSTCollider = _Collider<T, n, MeshSphereTree<T, n>>;
template<class T, uint n>
using PointCollider = _Collider<T, n, PointColliderBasis<T, n>>;
template<class T, uint n>
using RayCollider = _Collider<T, n, Ray<T, n>>;
template<class T, uint n>
using BoxCollider = _Collider<T, n, Box<T, n>>;
template<class T, uint n>
using TriangleCollider = _Collider<T, n, Triangle<T, n>>;

template<class T, uint n>
class Collider : public TransformableObject<T, n> {
private:
	CollisionQueue<T, n>* mQueue;
	uint64_t mFilters;
	bool mAsIs;

public:
	Collider(bool i_asIs = false);

	bool getAsIs() const;
	void setAsIs(bool i_value);
	uint64_t getFilters() const;
	void setFilters(uint64_t i_filters);
	CollisionQueue<T, n> * getQueue();
	void setQueue(CollisionQueue<T, n> * i_parent);

	virtual void applyTransform() = 0;
	virtual Collider<T, n> * clone() const = 0;
	virtual void getClosest(ClosestRequest<T, n>& in_request) const = 0;

	virtual void _accept1(Collider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(AAHalfSpaceCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(AABoxCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(SphereCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(HalfSpaceCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(LineCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(LineSegmentCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(MSTCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(PointCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(RayCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(BoxCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
	virtual void _accept2(TriangleCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) = 0;
};

template<class T, uint n, class Basis>
class _Collider : public Collider<T, n> {
public:
	using basis_t = Basis;
	using neuterable_ptr_t = neuterable_ptr<basis_t>;
	using neuterable_cptr_t = neuterable_ptr<const basis_t>;

private:
	basis_t mBasis;

public:
	_Collider(const basis_t& i_basis, bool i_asIs = false);
	virtual ~_Collider();

	basis_t& getBasis();
	const basis_t& getBasis() const;
	neuterable_ptr_t getTransformedBasis();
	neuterable_cptr_t getTransformedBasis() const;
	void applyTransform() override;
	Collider<T, n>* clone() const override;
	void getClosest(ClosestRequest<T, n>& in_request) const;


	void _accept1(Collider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(AAHalfSpaceCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(AABoxCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(SphereCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(HalfSpaceCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(LineCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(LineSegmentCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(MSTCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(PointCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(RayCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(BoxCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
	void _accept2(TriangleCollider<T, n>& i_other, Collision<T, n>& o_collision, collision_flags_t i_flags) override final;
};

template<class T, uint n, template<class, uint> class Basis>
_Collider<T, n, Basis<T, n>> makeCollider(Basis<T, n>& i_basis, bool i_asIs = false) {
	return _Collider<T, n, Basis<T, n>>(i_basis, i_asIs);
}

using Collider2f = Collider<float, 2>;
using Collider2d = Collider<double, 2>;
using Collider3f = Collider<float, 3>;
using Collider3d = Collider<double, 3>;
using AAHalfSpaceCollider2f = AAHalfSpaceCollider<float, 2>;
using AAHalfSpaceCollider2d = AAHalfSpaceCollider<double, 2>;
using AAHalfSpaceCollider3f = AAHalfSpaceCollider<float, 3>;
using AAHalfSpaceCollider3d = AAHalfSpaceCollider<double, 3>;
using AABoxCollider2f = AABoxCollider<float, 2>;
using AABoxCollider2d = AABoxCollider<double, 2>;
using AABoxCollider3f = AABoxCollider<float, 3>;
using AABoxCollider3d = AABoxCollider<double, 3>;
using SphereCollider2f = SphereCollider<float, 2>;
using SphereCollider2d = SphereCollider<double, 2>;
using SphereCollider3f = SphereCollider<float, 3>;
using SphereCollider3d = SphereCollider<double, 3>;
using HalfSpaceCollider2f = HalfSpaceCollider<float, 2>;
using HalfSpaceCollider2d = HalfSpaceCollider<double, 2>;
using HalfSpaceCollider3f = HalfSpaceCollider<float, 3>;
using HalfSpaceCollider3d = HalfSpaceCollider<double, 3>;
using LineCollider2f = LineCollider<float, 2>;
using LineCollider2d = LineCollider<double, 2>;
using LineCollider3f = LineCollider<float, 3>;
using LineCollider3d = LineCollider<double, 3>;
using LineSegmentCollider2f = Collider<float, 2>;
using LineSegmentCollider2d = Collider<double, 2>;
using LineSegmentCollider3f = Collider<float, 3>;
using LineSegmentCollider3d = Collider<double, 3>;
using MSTCollider2f = Collider<float, 2>;
using MSTCollider2d = Collider<double, 2>;
using MSTCollider3f = Collider<float, 3>;
using MSTCollider3d = Collider<double, 3>;
using PointCollider2f = Collider<float, 2>;
using PointCollider2d = Collider<double, 2>;
using PointCollider3f = Collider<float, 3>;
using PointCollider3d = Collider<double, 3>;
using RayCollider2f = Collider<float, 2>;
using RayCollider2d = Collider<double, 2>;
using RayCollider3f = Collider<float, 3>;
using RayCollider3d = Collider<double, 3>;
using BoxCollider2f = Collider<float, 2>;
using BoxCollider2d = Collider<double, 2>;
using BoxCollider3f = Collider<float, 3>;
using BoxCollider3d = Collider<double, 3>;
using TriangleCollider2f = Collider<float, 2>;
using TriangleCollider2d = Collider<double, 2>;
using TriangleCollider3f = Collider<float, 3>;
using TriangleCollider3d = Collider<double, 3>;

/*
#include "mesh_vertex_data.h"
#include "filtered_object.h"
#include "disableable_object.h"
#include "transformable_object.h"
#include "collision_queue.h"
#include <tuple>

#include "axis_aligned_half_space.h"
#include "axis_aligned_box.h"
#include "sphere.h"
#include "half_space.h"
#include "mesh_sphere_tree.h"
#include "line.h"
#include "line_segment.h"
#include "ray.h"
#include "box.h"
#include "triangle.h"

template<class Basis, class T, uint n>
class BasicCollider;

template<class T>
using AAHalfSpace2Collider = BasicCollider<AAHalfSpace<T, 2>, T, 2>;
template<class T>
using AARectangleCollider = BasicCollider<AABox<T, 2>, T, 2>;
template<class T>
using CircleCollider = BasicCollider<Sphere<T, 2>, T, 2>;
template<class T>
using HalfSpace2Collider = BasicCollider<HalfSpace<T, 2>, T, 2>;
template<class T>
using Line2Collider = BasicCollider<Line<T, 2>, T, 2>;
template<class T>
using LineSegment2Collider = BasicCollider<LineSegment<T, 2>, T, 2>;
template<class T>
using MST2Collider = BasicCollider<MeshSphereTree<T, 2>, T, 2>;
template<class T>
using Point2Collider = BasicCollider<Vector<T, 2>, T, 2>;
template<class T>
using Ray2Collider = BasicCollider<Ray<T, 2>, T, 2>;
template<class T>
using RectangleCollider = BasicCollider<Box<T, 2>, T, 2>;
template<class T>
using Triangle2Collider = BasicCollider<Triangle<T, 2>, T, 2>;

template<class T>
class _CollisionEvaluator;

template<class T, uint n>
class Collider
{};

template<class T>
class Collider<T, 2> : public FilteredObject, public DisableableObject, public TransformableObject<T, 2> {
protected:
	CollisionQueue<T, 2>* mQueue;
	bool mAsIs;

public:
	Collider(bool i_ignoreTransform = false);
	virtual ~Collider() {}

	bool getAsIs() const;
	void setAsIs(bool i_value);
	CollisionQueue<T, 2>* getQueue();
	void setQueue(CollisionQueue<T, 2>* i_parent);
	
	virtual void applyTransform() = 0;
	virtual Collider<T, 2>* clone() const = 0;
	virtual Vector<T, 2> getClosestPoint(const Vector<T, 2>& i_point) const = 0;
	virtual Vector<T, 2> getClosestNormal(const Vector<T, 2>& i_point, PointNormalPolicy i_policy = towards_point) const = 0;

	virtual Collision<T, 2> acceptEvaluator1(_CollisionEvaluator<T>& i_evaluator, Collider<T, 2>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, AAHalfSpace2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, AALine2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, AARectangleCollider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, CircleCollider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, HalfSpace2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, Line2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, LineSegment2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, MST2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, Point2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, Ray2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, RectangleCollider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptEvaluator2(_CollisionEvaluator<T>& i_evaluator, Triangle2Collider<T>& i_other) { throw NotImplementedException(); return Collision<T, 2>(); }
};

template<class T>
class Collider<T, 3> : public FilteredObject, public DisableableObject, public TransformableObject<T, 3> {
public:
	
};

using Collider2f = Collider<float, 2>;
using Collider2d = Collider<double, 2>;
using Collider3f = Collider<float, 3>;
using Collider3d = Collider<double, 3>;
*/
#endif


