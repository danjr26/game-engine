#ifndef COLLISION_EVALUATOR_H
#define COLLISION_EVALUATOR_H

#include "collision.h"
#include "exceptions.h"

template<uint>
class CollisionMask;
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
class CollisionEvaluator {
	Collision<double, n> Evaluate(CollisionMask<n>& in_mask1, CollisionMask<n>& in_mask2) = 0;
};

template<>
class CollisionEvaluator<2> {
public:
	Collision2d Evaluate(CollisionMask<2>& in_mask1, CollisionMask<2>& in_mask2);
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, AxisAlignedRectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, CircleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); };
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, HalfSpace2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, Line2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, Point2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, Ray2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, RectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

	virtual Collision2d Evaluate_Typed(CircleCollisionMask& in_mask1, CircleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(CircleCollisionMask& in_mask1, HalfSpace2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(CircleCollisionMask& in_mask1, Line2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(CircleCollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(CircleCollisionMask& in_mask1, Point2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(CircleCollisionMask& in_mask1, Ray2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(CircleCollisionMask& in_mask1, RectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(CircleCollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

	virtual Collision2d Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, HalfSpace2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, Line2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, Point2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, RectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(HalfSpace2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

	virtual Collision2d Evaluate_Typed(Line2CollisionMask& in_mask1, Line2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Line2CollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Line2CollisionMask& in_mask1, Point2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Line2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Line2CollisionMask& in_mask1, RectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Line2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

	virtual Collision2d Evaluate_Typed(LineSegment2CollisionMask& in_mask1, LineSegment2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(LineSegment2CollisionMask& in_mask1, Point2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(LineSegment2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(LineSegment2CollisionMask& in_mask1, RectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(LineSegment2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

	virtual Collision2d Evaluate_Typed(Point2CollisionMask& in_mask1, Point2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Point2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Point2CollisionMask& in_mask1, RectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Point2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

	virtual Collision2d Evaluate_Typed(Ray2CollisionMask& in_mask1, Ray2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Ray2CollisionMask& in_mask1, RectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(Ray2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

	virtual Collision2d Evaluate_Typed(RectangleCollisionMask& in_mask1, RectangleCollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }
	virtual Collision2d Evaluate_Typed(RectangleCollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

	virtual Collision2d Evaluate_Typed(Triangle2CollisionMask& in_mask1, Triangle2CollisionMask& in_mask2) { throw NotImplementedException(); return Collision2d(); }

};

using CollisionEvaluator2 = CollisionEvaluator<2>;
using CollisionEvaluator3 = CollisionEvaluator<3>;

#endif
