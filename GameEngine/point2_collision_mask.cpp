#include "point2_collision_mask.h"

Point2CollisionMask::Point2CollisionMask(const Vector2d& in_point, bool in_ignoreTransform) :
	CollisionMask2(in_ignoreTransform),
	point(in_point)
{}

Vector2d& Point2CollisionMask::Get_Point() {
	return point;
}

Vector2d Point2CollisionMask::Get_Transformed_Point() const {
	Vector2d out = point;
	if (!ignoreTransform) out = Vector2d(transform.Apply_To_Local_Point(Vector3d(out)));
	return out;
}

Collision2d Point2CollisionMask::Accept_Evaluator(CollisionEvaluator2* in_evaluator, CollisionMask2* in_other) {
	return in_other->Accept_Secondhand_Evaluator(in_evaluator, this);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, AxisAlignedRectangleCollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(in_other, this);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, CircleCollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(in_other, this);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, HalfSpace2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(in_other, this);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Line2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(in_other, this);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, LineSegment2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(in_other, this);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Point2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Ray2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, RectangleCollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}

Collision2d Point2CollisionMask::Accept_Secondhand_Evaluator(CollisionEvaluator2 * in_evaluator, Triangle2CollisionMask * in_other) {
	return in_evaluator->Evaluate_Typed(this, in_other);
}
