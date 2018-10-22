#include "axis_aligned_half_space2_collision_mask.h"

AxisAlignedHalfSpace2CollisionMask::AxisAlignedHalfSpace2CollisionMask(const AxisAlignedHalfSpace2d& in_halfSpace, bool in_ignoreTransform) :
	CollisionMask2(in_ignoreTransform),
	halfSpace(in_halfSpace)
{}

AxisAlignedHalfSpace2d& AxisAlignedHalfSpace2CollisionMask::Get_Half_Space() {
	return halfSpace;
}

AxisAlignedHalfSpace2d AxisAlignedHalfSpace2CollisionMask::Get_Transformed_Half_Space() {
	AxisAlignedHalfSpace2d out = halfSpace;
	if (!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Evaluator(CollisionEvaluator2& in_evaluator, CollisionMask2& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedHalfSpace2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedLine2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedRectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, CircleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, HalfSpace2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Line2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, LineSegment2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Point2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Ray2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, RectangleCollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

Collision2d AxisAlignedHalfSpace2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Triangle2CollisionMask& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}
