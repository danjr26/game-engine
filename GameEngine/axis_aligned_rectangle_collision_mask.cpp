#include "axis_aligned_rectangle_collision_mask.h"

AxisAlignedRectangleCollisionMask::AxisAlignedRectangleCollisionMask(const AxisAlignedRectangled& in_rectangle, bool in_ignoreTransform) :
	CollisionMask2(in_ignoreTransform),
	rectangle(in_rectangle)
{}

AxisAlignedRectangled& AxisAlignedRectangleCollisionMask::Get_Rectangle() {
	return rectangle;
}

AxisAlignedRectangled AxisAlignedRectangleCollisionMask::Get_Transformed_Rectangle() {
	AxisAlignedRectangled out = rectangle;
	if(!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Evaluator(CollisionEvaluator2& in_evaluator, CollisionMask& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedRectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, CircleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, HalfSpace2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Line2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, LineSegment2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Point2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Ray2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, RectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedRectangleCollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Triangle2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}
