#include "half_space2_collision_mask.h"

HalfSpace2CollisionMask::HalfSpace2CollisionMask(const HalfSpace2d& in_halfSpace, bool in_ignoreTransform) :
	CollisionMask2(in_ignoreTransform),
	halfSpace(in_halfSpace)
{}

HalfSpace2d& HalfSpace2CollisionMask::Get_Half_Space() {
	return halfSpace;
}

HalfSpace2d HalfSpace2CollisionMask::Get_Transformed_Half_Space() {
	HalfSpace2d out = halfSpace;
	if (!ignoreTransform) halfSpace.Apply_Transform(transform);
	return out;
}

Collision2d HalfSpace2CollisionMask::Accept_Evaluator(CollisionEvaluator2& in_evaluator, CollisionMask2& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedRectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, CircleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, HalfSpace2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Line2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, LineSegment2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Point2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Ray2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, RectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d HalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Triangle2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}
