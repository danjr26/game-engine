#ifndef COLLISION_MASK_H
#define COLLISION_MASK_H

#include "mesh_vertex_data.h"
#include "filtered_object.h"
#include "disableable_object.h"
#include "transformable_object.h"
#include "collision_queue.h"
#include <tuple>

#include "axis_aligned_half_space.h"
#include "axis_aligned_line.h"
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
class BasicCollisionMask;

template<class T>
using AAHalfSpace2CollisionMask = BasicCollisionMask<AxisAlignedHalfSpace<T, 2>, T, 2>;
template<class T>
using AALine2CollisionMask = BasicCollisionMask<AxisAlignedLine<T, 2>, T, 2>;
template<class T>
using AARectangleCollisionMask = BasicCollisionMask<AxisAlignedBox<T, 2>, T, 2>;
template<class T>
using CircleCollisionMask = BasicCollisionMask<Sphere<T, 2>, T, 2>;
template<class T>
using HalfSpace2CollisionMask = BasicCollisionMask<HalfSpace<T, 2>, T, 2>;
template<class T>
using Line2CollisionMask = BasicCollisionMask<Line<T, 2>, T, 2>;
template<class T>
using LineSegment2CollisionMask = BasicCollisionMask<LineSegment<T, 2>, T, 2>;
template<class T>
using MST2CollisionMask = BasicCollisionMask<MeshSphereTree<T, 2>, T, 2>;
template<class T>
using Point2CollisionMask = BasicCollisionMask<Vector<T, 2>, T, 2>;
template<class T>
using Ray2CollisionMask = BasicCollisionMask<Ray<T, 2>, T, 2>;
template<class T>
using RectangleCollisionMask = BasicCollisionMask<Box<T, 2>, T, 2>;
template<class T>
using Triangle2CollisionMask = BasicCollisionMask<Triangle<T, 2>, T, 2>;

template<class T, uint n>
class CollisionEvaluator;

template<class T, uint n>
class CollisionMask
{};

template<class T>
class CollisionMask<T, 2> : public FilteredObject, public DisableableObject, public TransformableObject<T, 2> {
protected:
	CollisionQueue<T, 2>* mQueue;
	bool mIgnoreTransform;

public:
	enum PointNormalPolicy {
		zero,
		nearest_edge,
		towards_point
	};

public:
	CollisionMask(bool in_ignoreTransform = false);
	virtual ~CollisionMask() {}

	bool getIgnoreTransform() const;
	void setIgnoreTransform(bool in_value);
	CollisionQueue<T, 2>* getQueue();
	void setQueue(CollisionQueue<T, 2>* in_parent);
	
	virtual void applyTransform() = 0;
	virtual CollisionMask<T, 2>* clone() const = 0;
	virtual Vector<T, 2> getClosestPoint(const Vector<T, 2>& in_point) const = 0;
	virtual Vector<T, 2> getClosestNormal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy = towards_point) const = 0;

	virtual Collision<T, 2> acceptEvaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, AAHalfSpace2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, AALine2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, AARectangleCollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, MST2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
};

template<class T>
class CollisionMask<T, 3> : public FilteredObject, public DisableableObject, public TransformableObject<T, 3> {
public:
	
};

using CollisionMask2f = CollisionMask<float, 2>;
using CollisionMask2d = CollisionMask<double, 2>;
using CollisionMask3f = CollisionMask<float, 3>;
using CollisionMask3d = CollisionMask<double, 3>;

#endif


