#ifndef COLLISION_MASK_H
#define COLLISION_MASK_H

#include "mesh_vertex_data.h"
#include "filtered_object.h"
#include "disableable_object.h"
#include "collision.h"
#include "collision_evaluator.h"
#include "transformable_object.h"
#include <tuple>

class AxisAlignedRectangleCollisionMask;
class CircleCollisionMask;
class HalfSpace2CollisionMask;
class Line2CollisionMask;
class LineSegment2CollisionMask;
class Point2CollisionMask;
class Ray2CollisionMask;
class RectangleCollisionMask;
class Triangle2CollisionMask;

template<uint n>
class CollisionMask : public FilteredObject, public DisableableObject, public TransformableObject {};

template<>
class CollisionMask<2> : public FilteredObject, public DisableableObject, public TransformableObject {
public:
	virtual Collision2d Accept_Evaluator(CollisionEvaluator2* in_evaluator, CollisionMask<2>* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, AxisAlignedRectangleCollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, CircleCollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, HalfSpace2CollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Line2CollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, LineSegment2CollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Point2CollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Ray2CollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, RectangleCollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Triangle2CollisionMask* in_other) { throw NotImplementedException(); return Collision2d(); }
};

template<>
class CollisionMask<3> : public FilteredObject, public DisableableObject, public TransformableObject {
public:
	
};

using CollisionMask2 = CollisionMask<2>;
using CollisionMask3 = CollisionMask<3>;

#endif