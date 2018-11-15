#ifndef COLLISION_MASK_H
#define COLLISION_MASK_H

#include "mesh_vertex_data.h"
#include "filtered_object.h"
#include "disableable_object.h"
#include "collision.h"
#include "collision_evaluator.h"
#include "transformable_object.h"
#include <tuple>

template<class T>
class AxisAlignedHalfSpace2CollisionMask;
template<class T>
class AxisAlignedLine2CollisionMask;
template<class T>
class AxisAlignedRectangleCollisionMask;
template<class T>
class CircleCollisionMask;
template<class T>
class HalfSpace2CollisionMask;
template<class T>
class Line2CollisionMask;
template<class T>
class LineSegment2CollisionMask;
template<class T>
class Point2CollisionMask;
template<class T>
class Ray2CollisionMask;
template<class T>
class RectangleCollisionMask;
template<class T>
class Triangle2CollisionMask;

template<class T, uint n>
class CollisionMask
{};

template<class T>
class CollisionMask<T, 2> : public FilteredObject, public DisableableObject, public TransformableObject<T, 2> {
protected:
	bool ignoreTransform;
	void* parent;

public:
	CollisionMask(bool in_ignoreTransform = false) : 
		ignoreTransform(in_ignoreTransform),
		parent(nullptr)
	{}

	void Set_Ignore_Transform(bool in_value) { 
		ignoreTransform = in_value;
	}

	void Set_Parent(void* in_parent) {
		parent = in_parent;
	}

	void* Get_Parent() {
		return parent;
	}

	virtual void Apply_Transform() = 0;
	virtual CollisionMask<T, 2>* Clone() const = 0;
	virtual Vector<T, 2> Get_Closest_Point(const Vector<T, 2>& in_point) const = 0;
	virtual Vector<T, 2> Get_Closest_Normal(const Vector<T, 2>& in_point) const = 0;

	virtual Collision<T, 2> Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
	virtual Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) { throw NotImplementedException(); return Collision<T, 2>(); }
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
