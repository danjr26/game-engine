#include "triangle2_collision_mask.h"

Triangle2CollisionMask::Triangle2CollisionMask(const Triangle2d& in_triangle, bool in_ignoreTransform) :
	CollisionMask2(in_ignoreTransform),
	triangle(in_triangle)
{}

Triangle2d& Triangle2CollisionMask::Get_Triangle() {
	return triangle;
}

Triangle2d Triangle2CollisionMask::Get_Transformed_Triangle() {
	Triangle2d out = triangle;
	if (!ignoreTransform) triangle.Apply_Transform(transform);
	return out;	
}

Collision2d Triangle2CollisionMask::Accept_Evaluator(CollisionEvaluator2& in_evaluator, CollisionMask2& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedHalfSpace2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedLine2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedRectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, CircleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, HalfSpace2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Line2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, LineSegment2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Point2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Ray2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, RectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d Triangle2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Triangle2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}
