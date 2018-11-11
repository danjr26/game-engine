#ifndef IN_PLACE_COLLISION_EVALUATOR2_H
#define IN_PLACE_COLLISION_EVALUATOR2_H

#include "collision_evaluator.h"
#include "axis_aligned_half_space2_collision_mask.h"
#include "axis_aligned_line2_collision_mask.h"
#include "axis_aligned_rectangle_collision_mask.h"
#include "circle_collision_mask.h"
#include "half_space2_collision_mask.h"
#include "line2_collision_mask.h"
#include "line_segment2_collision_mask.h"
#include "point2_collision_mask.h"
#include "ray2_collision_mask.h"
#include "rectangle_collision_mask.h"
#include "triangle2_collision_mask.h"

template<class T, uint n>
class InPlaceCollisionEvaluator
{};

template<class T>
class InPlaceCollisionEvaluator<T, 2> : public CollisionEvaluator<T, 2> {	
public:
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, AxisAlignedHalfSpace2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, AxisAlignedLine2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, AxisAlignedRectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedHalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, AxisAlignedRectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(AxisAlignedRectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(CircleCollisionMask<T>& in_mask1, CircleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(CircleCollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(CircleCollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(CircleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(CircleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, HalfSpace2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(HalfSpace2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Line2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Line2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Line2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Line2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, LineSegment2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(LineSegment2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Point2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Point2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Point2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, Ray2CollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(Ray2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(RectangleCollisionMask<T>& in_mask1, RectangleCollisionMask<T>& in_mask2) override;
	Collision<T, 2> Evaluate_Typed(RectangleCollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;

	Collision<T, 2> Evaluate_Typed(Triangle2CollisionMask<T>& in_mask1, Triangle2CollisionMask<T>& in_mask2) override;
};

using InPlaceCollisionEvaluator2f = InPlaceCollisionEvaluator<float, 2>;
using InPlaceCollisionEvaluator2d = InPlaceCollisionEvaluator<double, 2>;
using InPlaceCollisionEvaluator3f = InPlaceCollisionEvaluator<float, 3>;
using InPlaceCollisionEvaluator3d = InPlaceCollisionEvaluator<double, 3>;

#endif	