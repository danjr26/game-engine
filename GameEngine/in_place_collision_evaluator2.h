#ifndef IN_PLACE_COLLISION_EVALUATOR2_H
#define IN_PLACE_COLLISION_EVALUATOR2_H

#include "collision_evaluator.h"
#include "axis_aligned_rectangle_collision_mask.h"
#include "circle_collision_mask.h"
#include "half_space2_collision_mask.h"
#include "line2_collision_mask.h"
#include "line_segment2_collision_mask.h"
#include "point2_collision_mask.h"
#include "ray2_collision_mask.h"
#include "rectangle_collision_mask.h"
#include "triangle2_collision_mask.h"

class InPlaceCollisionEvaluator2 : public CollisionEvaluator2 {
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, AxisAlignedRectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, CircleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, HalfSpace2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, Line2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, LineSegment2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, Point2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, Ray2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, RectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;

	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, CircleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, HalfSpace2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, Line2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, LineSegment2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, Point2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, Ray2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, RectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;

	Collision2d Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, HalfSpace2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, Line2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, LineSegment2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, Point2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, Ray2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, RectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(HalfSpace2CollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;

	Collision2d Evaluate_Typed(Line2CollisionMask* in_mask1, Line2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Line2CollisionMask* in_mask1, LineSegment2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Line2CollisionMask* in_mask1, Point2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Line2CollisionMask* in_mask1, Ray2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Line2CollisionMask* in_mask1, RectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Line2CollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;

	Collision2d Evaluate_Typed(LineSegment2CollisionMask* in_mask1, LineSegment2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(LineSegment2CollisionMask* in_mask1, Point2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(LineSegment2CollisionMask* in_mask1, Ray2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(LineSegment2CollisionMask* in_mask1, RectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(LineSegment2CollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;

	Collision2d Evaluate_Typed(Point2CollisionMask* in_mask1, Point2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Point2CollisionMask* in_mask1, Ray2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Point2CollisionMask* in_mask1, RectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Point2CollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;

	Collision2d Evaluate_Typed(Ray2CollisionMask* in_mask1, Ray2CollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Ray2CollisionMask* in_mask1, RectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(Ray2CollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;

	Collision2d Evaluate_Typed(RectangleCollisionMask* in_mask1, RectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(RectangleCollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;

	Collision2d Evaluate_Typed(Triangle2CollisionMask* in_mask1, Triangle2CollisionMask* in_mask2) override;
};

#endif	