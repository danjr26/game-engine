#include "rectangle_collision_mask.h"

RectangleCollisionMask::RectangleCollisionMask(const Rectangled& in_rectangle, bool in_ignoreTransform) :
	CollisionMask2(in_ignoreTransform),
	rectangle(in_rectangle)
{}

Rectangled& RectangleCollisionMask::Get_Rectangle() {
	return rectangle;
}

Rectangled RectangleCollisionMask::Get_Transformed_Rectangle() {
	Rectangled out = rectangle;
	if (!ignoreTransform) rectangle.Apply_Transform(transform);
	return out;
}

Collision2d RectangleCollisionMask::Accept_Evaluator(CollisionEvaluator2& in_evaluator, CollisionMask2& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedHalfSpace2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedLine2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedRectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, CircleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, HalfSpace2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Line2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, LineSegment2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Point2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Ray2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, RectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d RectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Triangle2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}
